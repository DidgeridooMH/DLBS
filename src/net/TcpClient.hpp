#pragma once

#include <memory>
#include <multithread/HotThread.hpp>
#include <net/NetLayer.hpp>
#include <stack>
#include <vector>

namespace dlbs {
class TcpClient : public HotThread, NetLayer {
 public:
  TcpClient(int fd, const std::string& address, uint16_t port);
  ~TcpClient();

 protected:
  void RunHandler() override;

 private:
  int m_fd;
  std::string m_address;
  uint16_t m_port;
};
}  // namespace dlbs
