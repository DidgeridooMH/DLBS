#include <fmt/printf.h>

#include <csignal>
#include <net/TcpServer.hpp>

void waitUtilInterrupt() {
  static std::condition_variable cv;
  std::mutex m;
  std::unique_lock lock(m);
  std::signal(SIGINT, [](auto) { cv.notify_all(); });
  std::signal(SIGTERM, [](auto) { cv.notify_all(); });
  cv.wait(lock);
}

class DummyClient : public dlbs::TcpClient {
 public:
  DummyClient(int fd) : TcpClient(fd) {}

 protected:
  void OnMessage(const std::vector<uint8_t>& __attribute__((unused)) buffer,
                 size_t length) override {
    fmt::print("LENGTH => {}\n", length);
  }
};

int main() {
  fmt::print("DidgeridooMH's Load Balancing Service 🤪 v{}\n",
             PROJECT_VERSION);

  auto serverThread = dlbs::TcpServer<DummyClient>(8080);
  serverThread.Run();

  waitUtilInterrupt();

  return 0;
}
