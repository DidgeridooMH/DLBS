#pragma once

#include <cstdint>

namespace dlbs {
struct EndPointContext {
  bool keepAlive;
  uint32_t endPointAddress;
  uint16_t endPointPort;
};
}  // namespace dlbs
