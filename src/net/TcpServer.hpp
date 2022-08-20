#pragma once

#include <fmt/format.h>

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <memory>
#include <multithread/HotThread.hpp>
#include <net/TcpClient.hpp>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

extern "C" {
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
}

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
  std::unordered_map<uint16_t, std::unique_ptr<TcpClient>> m_clients;
};

}  // namespace dlbs
