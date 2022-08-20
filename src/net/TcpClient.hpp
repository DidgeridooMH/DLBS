#pragma once

#include <memory>
#include <multithread/HotThread.hpp>
#include <optional>
#include <vector>

namespace dlbs {
typedef std::vector<uint8_t> NetBuffer;
typedef std::function<bool(NetBuffer&, size_t)> NetFilter;

class TcpClient : public HotThread {
 public:
  TcpClient(int fd, const std::string& address, uint16_t port);

 protected:
  void RunHandler() override;

 private:
  int m_fd;
  std::string m_address;
  uint16_t m_port;
  std::vector<NetFilter> m_filters;
};
}  // namespace dlbs
