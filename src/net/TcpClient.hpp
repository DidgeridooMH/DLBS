#pragma once

#include <multithread/HotThread.hpp>
#include <vector>

namespace dlbs {
class TcpClient : public HotThread {
 public:
  TcpClient(int fd);

  virtual ~TcpClient();

 protected:
  virtual void OnMessage(const std::vector<uint8_t>& buffer, size_t length) = 0;

  void RunHandler() override;

 private:
  int m_fd;
};
}  // namespace dlbs
