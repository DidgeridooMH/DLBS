#pragma once

#include <memory>
#include <multithread/HotThread.hpp>
#include <net/NetUtils.hpp>
#include <optional>
#include <vector>

namespace dlbs {
class TcpClient : public HotThread {
 public:
  TcpClient(int fd, const std::string& address, uint16_t port);

  void AddFilter(NetFilter filter);

 protected:
  void RunHandler() override;

 private:
  int m_fd;
  std::string m_address;
  uint16_t m_port;
  std::vector<NetFilter> m_filters;
};
}  // namespace dlbs
