#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "multithread/HotThread.hpp"
#include "net/NetUtils.hpp"

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
  EndPointContext m_context;
};
}  // namespace dlbs
