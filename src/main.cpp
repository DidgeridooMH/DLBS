#include <fmt/printf.h>

#include <csignal>

#include "config/Config.hpp"
#include "net/tcp/TcpServer.hpp"

void waitUtilInterrupt() {
  static std::condition_variable cv;
  std::mutex m;
  std::unique_lock lock(m);
  std::signal(SIGINT, [](auto) { cv.notify_all(); });
  std::signal(SIGTERM, [](auto) { cv.notify_all(); });
  cv.wait(lock);
}

int main(int argc, char** argv) {
  fmt::print("DidgeridooMH's Load Balancing Service 🤪 v{}\n",
             PROJECT_VERSION);

  if (argc > 1) {
    std::string filename = argv[1];
    dlbs::Config::GetInstance()->Load(filename);
  } else {
    dlbs::Config::GetInstance()->Load("/etc/dlbs.json");
  }

  auto serverThread = dlbs::TcpServer(8080);
  serverThread.Run();

  waitUtilInterrupt();

  return 0;
}
