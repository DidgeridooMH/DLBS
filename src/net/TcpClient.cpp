#include <net/TcpClient.hpp>

extern "C" {
#include <unistd.h>
}

namespace dlbs {
TcpClient::TcpClient(int fd) : m_fd(fd) {}

TcpClient::~TcpClient() { close(m_fd); }

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
