#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <vector>

namespace dlbs {
typedef std::vector<char> NetBuffer;
typedef std::function<void(NetBuffer&, size_t)> NetFilter;

struct InternalServerException : public std::exception {
  const char* what() const noexcept { return "Internal Server Error"; }
};

struct NotEnoughDataException : public std::exception {
  const char* what() const noexcept { return "Not enough data"; }
};
}  // namespace dlbs
