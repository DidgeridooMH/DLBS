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

void TcpClient::RunHandler() {
  std::vector<uint8_t> buffer(16 * 1024);
  size_t size = 0;

  while (GetState() != ThreadState::Stopping) {
    auto len = read(m_fd, buffer.data() + size, buffer.size() - size);
    if (len > 0) {
      bool filterHealth = true;
      for (const auto& f : m_filters) {
        filterHealth = f(buffer, size);
        if (!filterHealth) {
          break;
        }
      }
      if (filterHealth) {
        std::string message = "Passed all health checks!\n";
        write(m_fd, message.c_str(), message.length());

        // TODO: keep-alive
        break;
      }
    } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
      break;
    }

    std::this_thread::yield();
  }

  close(m_fd);
  fmt::print("[DEBUG] Client disconnected at {}:{}\n", m_address, m_port);
}
}  // namespace dlbs
