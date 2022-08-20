#include <fmt/format.h>

#include <net/TcpClient.hpp>

extern "C" {
#include <unistd.h>
}

namespace dlbs {
TcpClient::TcpClient(int fd, const std::string& address, uint16_t port)
    : m_fd(fd), m_address(address), m_port(port) {
  // TODO: Make debug logging
  fmt::print("[DEBUG] - Client connected at {}:{}\n", address, port);
}

TcpClient::~TcpClient() {
  close(m_fd);
  fmt::print("[DEBUG] Client disconnected at {}:{}\n", m_address, m_port);
}

void TcpClient::RunHandler() {
  std::vector<uint8_t> buffer(4096);

  while (GetState() != ThreadState::Stopping) {
    auto len = read(m_fd, buffer.data(), buffer.size());
    if (len > 0) {
      OnMessage(buffer, len);
    } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
      break;
    }

    std::this_thread::yield();
  }
}
}  // namespace dlbs
