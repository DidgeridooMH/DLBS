#pragma once

#include <fmt/format.h>

#include <chrono>
#include <cstdint>
#include <cstring>
#include <net/NetUtils.hpp>
#include <stdexcept>
#include <string>

extern "C" {
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
}

using namespace std::chrono;
using namespace std::chrono_literals;

namespace dlbs {
class EndPointConnection {
 public:
  EndPointConnection(const std::string& address, uint16_t port) {
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd < 0) {
      throw std::runtime_error(
          fmt::format("Unable to create socket: {}", strerror(errno)));
    }

    sockaddr_in sockAddress = {
        .sin_addr = {.s_addr = inet_addr(address.c_str())},
        .sin_family = AF_INET,
        .sin_len = sizeof(sockaddr_in),
        .sin_port = htons(port),
        .sin_zero = {0}};

    if (connect(m_fd, reinterpret_cast<sockaddr*>(&sockAddress),
                sizeof(sockAddress)) < 0) {
      throw std::runtime_error(fmt::format("Unable to connect to address: {}",
                                           std::strerror(errno)));
    }

    auto flags = fcntl(m_fd, F_GETFL);
    fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
  }

  ~EndPointConnection() { close(m_fd); }

  void Send(const NetBuffer& buffer, size_t size) {
    size_t sent = 0;
    while (sent < size) {
      auto bytesSent = write(m_fd, buffer.data() + sent, size - sent);
      if (bytesSent <= 0) {
        throw std::runtime_error(fmt::format(
            "Unable to send data to endpoint: {}", strerror(errno)));
      }
      sent += bytesSent;
    }
  }

  NetBuffer Receive() {
    NetBuffer buffer(2 * 1024 * 1024);
    size_t size = 0;

    auto startTime = steady_clock::now();
    while (size < buffer.size()) {
      if ((steady_clock::now() - startTime) > 3s) {
        throw std::runtime_error("Endpoint timed out");
      }

      auto bytesRead = read(m_fd, buffer.data() + size, buffer.size() - size);
      if (bytesRead > 0) {
        size += bytesRead;

        try {
          for (auto& f : m_filters) {
            f(buffer, size);
          }
        } catch (const NotEnoughDataException& e) {
          continue;
        }
      } else if (bytesRead == 0) {
        break;
      } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
        throw std::runtime_error(
            fmt::format("Unable to read from endpoint: {}", strerror(errno)));
      }
    }

    buffer.resize(size);
    return buffer;
  }

  void AddFilter(NetFilter filter) { m_filters.push_back(filter); }

 private:
  int m_fd;
  std::vector<NetFilter> m_filters;
};
}  // namespace dlbs
