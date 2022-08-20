#pragma once

#include <cstdint>
#include <multithread/HotThread.hpp>

namespace dlbs {
class TcpServer : public HotThread {
 public:
  TcpServer(uint16_t port);
  ~TcpServer();

 protected:
  void RunHandler() override;

 private:
  uint16_t m_port;
  int m_fd;
};

}  // namespace dlbs
