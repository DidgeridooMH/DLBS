#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <net/endpoint/EndPointContext.hpp>
#include <string>
#include <vector>

namespace dlbs {
typedef std::vector<char> NetBuffer;
typedef std::function<void(NetBuffer&, size_t, EndPointContext& context)>
    NetFilter;

struct InternalServerException : public std::exception {
  InternalServerException(const std::string& msg) : message(msg) {}

  const char* what() const noexcept { return message.c_str(); }

  std::string message;
};

struct NotEnoughDataException : public std::exception {
  const char* what() const noexcept { return "Not enough data"; }
};
}  // namespace dlbs
