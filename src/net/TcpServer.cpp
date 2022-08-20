#include <net/TcpServer.hpp>
#include <net/filters/HttpFilter.hpp>

extern "C" {
#include <arpa/inet.h>
}

namespace dlbs {
TcpServer::TcpServer(uint16_t port) : m_port(port) {
  m_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_fd < 0) {
    throw std::runtime_error("Unable to make socket.");
  }

  int opt = 1;
  if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
    throw std::runtime_error(
        fmt::format("Unable to set socket flags: {}", strerror(errno)));
  }

  auto flags = fcntl(m_fd, F_GETFL);
  fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);

  sockaddr_in address = {
      .sin_family = AF_INET,
      .sin_addr = {.s_addr = htonl(INADDR_ANY)},
      .sin_len = sizeof(sockaddr_in),
      .sin_port = htons(port),
      .sin_zero = {0},
  };

  if (bind(m_fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) !=
      0) {
    throw std::runtime_error("Unable to bind socket.");
  }
}

TcpServer::~TcpServer() {
  m_clients.clear();
  close(m_fd);
}

void TcpServer::RunHandler() {
  if (listen(m_fd, 3) < 0) {
    throw std::runtime_error(
        fmt::format("Unable to set listen state: {}", strerror(errno)));
  }

  fmt::print("Server started on port {}\n", m_port);

  while (GetState() != ThreadState::Stopping) {
    sockaddr_in clientAddress;
    socklen_t addressLength = sizeof(clientAddress);
    int clientFd = accept(m_fd, reinterpret_cast<sockaddr *>(&clientAddress),
                          &addressLength);
    if (clientFd >= 0) {
      auto client = std::make_unique<TcpClient>(
          clientFd, inet_ntoa(clientAddress.sin_addr),
          ntohs(clientAddress.sin_port));
      client->AddFilter(filters::http);
      m_clients.insert({clientAddress.sin_port, std::move(client)});
      m_clients.at(clientAddress.sin_port)->Run();
    } else if (errno != EWOULDBLOCK && errno != EAGAIN) {
      break;
    }

    std::this_thread::yield();
  }

  fmt::print("Server stopped\n");
}

}  // namespace dlbs
