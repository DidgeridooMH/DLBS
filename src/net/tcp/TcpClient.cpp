#include "net/tcp/TcpClient.hpp"

#include <fmt/format.h>

#include <exception>

#include "net/NetUtils.hpp"
#include "net/endpoint/EndPointConnection.hpp"
#include "net/filters/HttpFilter.hpp"

extern "C" {
#include <unistd.h>
}

using namespace std::chrono;
using namespace std::chrono_literals;

namespace dlbs {
TcpClient::TcpClient(int fd, const std::string& address, uint16_t port,
                     uint32_t endPointAddress, uint16_t endPointPort)
    : m_fd(fd),
      m_address(address),
      m_port(port),
      m_context(EndPointContext{.keepAlive = false,
                                .endPointAddress = endPointAddress,
                                .endPointPort = endPointPort}) {
  fmt::print("[DEBUG] - Client connected at {}:{}\n", address, port);
}

void TcpClient::AddFilter(NetFilter filter) { m_filters.push_back(filter); }

void TcpClient::RunHandler() {
  NetBuffer buffer(16 * 1024);
  size_t size = 0;

  auto timeout = steady_clock::now();
  while (GetState() != ThreadState::Stopping && size != buffer.size() &&
         (steady_clock::now() - timeout) < 5s) {
    auto len = read(m_fd, buffer.data() + size, buffer.size() - size);
    if (len > 0) {
      timeout = steady_clock::now();
      size += len;
      try {
        for (const auto& f : m_filters) {
          f(buffer, size, m_context);
        }

        auto endPointConnection = EndPointConnection(m_context.endPointAddress,
                                                     m_context.endPointPort);
        endPointConnection.AddFilter(filters::http);
        endPointConnection.Send(buffer, size);

        auto message = endPointConnection.Receive();
        write(m_fd, message.data(), message.size());

      } catch (const InternalServerException& e) {
        fmt::print("Internal server error: {}\n", e.what());
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
      } catch (const std::exception& e) {
        fmt::print("Server error: {}\n", e.what());
        break;
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

      if (m_context.keepAlive) {
        size = 0;
      } else {
        break;
      }
    } else if (len == 0 && errno != EWOULDBLOCK && errno != EAGAIN) {
      break;
    }

    std::this_thread::yield();
  }

  close(m_fd);
  fmt::print("[DEBUG] Client disconnected at {}:{}\n", m_address, m_port);
}
}  // namespace dlbs
