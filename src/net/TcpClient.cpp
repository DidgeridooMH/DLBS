#include <fmt/format.h>

#include <net/NetUtils.hpp>
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

void TcpClient::AddFilter(NetFilter filter) { m_filters.push_back(filter); }

void TcpClient::RunHandler() {
  NetBuffer buffer(16 * 1024);
  size_t size = 0;

  while (GetState() != ThreadState::Stopping && size != buffer.size()) {
    auto len = read(m_fd, buffer.data() + size, buffer.size() - size);
    if (len > 0) {
      size += len;
      try {
        for (const auto& f : m_filters) {
          f(buffer, size);
        }
      } catch (const InternalServerException& e) {
        std::string iseDocument =
            "<!DOCTYPE html><html><head><title>Internal Server "
            "Error</title></head><body><h1>Oops something went "
            "wrong</h1></body></html>";
        std::string iseMessage = fmt::format(
            "HTTP/1.1 500 INTERNAL_SERVER_ERROR\n"
            "Content-Length: {}\n"
            "\n"
            "{}",
            iseDocument.size(), iseDocument);
        write(m_fd, iseMessage.c_str(), iseMessage.length());
        break;
      } catch (const NotEnoughDataException) {
        continue;
      }

      std::string message =
          "<!DOCTYPE html><html><body><h1>Passed all health "
          "checks!</h1></body></html>\n";
      std::string request = fmt::format(
          "HTTP/1.1 200 OK\n"
          "Content-Length: {}\n"
          "\n"
          "{}",
          message.length(), message);
      write(m_fd, request.c_str(), request.length());

      break;
    } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
      break;
    }

    std::this_thread::yield();
  }

  close(m_fd);
  fmt::print("[DEBUG] Client disconnected at {}:{}\n", m_address, m_port);
}
}  // namespace dlbs
