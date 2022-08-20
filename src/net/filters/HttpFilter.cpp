#include <fmt/format.h>

#include <net/filters/HttpFilter.hpp>

#include "net/NetUtils.hpp"

namespace dlbs::filters {
static size_t checkRequestLine(NetBuffer& buffer, size_t size) {
  size_t cursor = 1;
  int whitespaceCount = 0;
  while (cursor < size && buffer[cursor] != '\n') {
    if (buffer[cursor] != '\r' && isspace(buffer[cursor]) &&
        !isspace(buffer[cursor - 1])) {
      whitespaceCount += 1;
    }
    cursor += 1;
  }

  if (cursor == size) {
    throw NotEnoughDataException();
  } else if (whitespaceCount != 2) {
    throw InternalServerException();
  }

  return cursor + 1;
}

static size_t parseHeaderField(NetBuffer& buffer, size_t size, size_t cursor,
                               HeaderOptions& options) {
  auto start = cursor;
  while (cursor < size && buffer[cursor] != ':' && buffer[cursor] != '\n') {
    cursor += 1;
  }

  if (cursor == size) {
    throw NotEnoughDataException();
  } else if (buffer[cursor] != ':') {
    throw InternalServerException();
  }

  std::string key(buffer.begin() + start, buffer.begin() + cursor);
  if (key[key.size() - 1] == '\r') {
    key = key.substr(0, key.size() - 1);
  }
  std::for_each(key.begin(), key.end(), std::tolower);

  cursor += 1;
  while (cursor < size && buffer[cursor] != '\n' && isspace(buffer[cursor])) {
    cursor += 1;
  }

  if (cursor == size) {
    throw NotEnoughDataException();
  }

  start = cursor;
  while (cursor < size && buffer[cursor] != '\n') {
    cursor += 1;
  }

  if (cursor == size) {
    throw NotEnoughDataException();
  }

  std::string value(buffer.begin() + start, buffer.begin() + cursor);
  if (value[value.size() - 1] == '\r') {
    value = value.substr(0, value.size() - 1);
  }
  if (value.empty()) {
    throw InternalServerException();
  }

  if (options.contains(key)) {
    options[key] = value;
  } else {
    options.insert({key, value});
  }

  return cursor + 1;
}

static size_t parseHeaderOptions(NetBuffer& buffer, size_t size, size_t cursor,
                                 HeaderOptions& options) {
  while (cursor < size && buffer[cursor] != '\n') {
    if (buffer[cursor] != '\r') {
      cursor = parseHeaderField(buffer, size, cursor, options);
    } else {
      cursor++;
    }
  }
  if (cursor == size) {
    throw NotEnoughDataException();
  }
  return cursor + 1;
}

void http(NetBuffer& buffer, size_t size) {
  auto cursor = checkRequestLine(buffer, size);

  HeaderOptions headerOptions;
  cursor = parseHeaderOptions(buffer, size, cursor, headerOptions);
  if (!headerOptions.contains("content-length")) {
    headerOptions.insert({"content-length", "0"});
  }

  auto& lengthOption = headerOptions["content-length"];
  if (std::find_if_not(lengthOption.begin(), lengthOption.end(),
                       std::isdigit) != lengthOption.end()) {
    throw InternalServerException();
  }
  size_t length = std::stoi(lengthOption);
  if ((size - cursor) < length) {
    throw NotEnoughDataException();
  }
}
}  // namespace dlbs::filters
