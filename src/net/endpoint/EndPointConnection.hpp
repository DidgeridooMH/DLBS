#pragma once

#include <net/NetUtils.hpp>
#include <string>

namespace dlbs {
class EndPointConnection {
 public:
  EndPointConnection(uint32_t address, uint16_t port);

  ~EndPointConnection();

  void Send(const NetBuffer& buffer, size_t size);

  NetBuffer Receive();

  void AddFilter(NetFilter filter);

 private:
  int m_fd;
  std::vector<NetFilter> m_filters;
};
}  // namespace dlbs
