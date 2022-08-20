#pragma once

#include <thread>

namespace dlbs {

enum class ThreadState { Running, Stopping, Stopped };

class HotThread {
 public:
  HotThread();

  virtual ~HotThread();

  void Run();

  ThreadState GetState() const;

 protected:
  virtual void RunHandler() = 0;

 private:
  std::thread m_thread;
  ThreadState m_state;
};
}  // namespace dlbs
