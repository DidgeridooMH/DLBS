#include "multithread/HotThread.hpp"

namespace dlbs {
HotThread::HotThread() : m_state(ThreadState::Stopped) {}

HotThread::~HotThread() {
  if (m_state == ThreadState::Running) {
    m_state = ThreadState::Stopping;
    m_thread.join();
  }
}

ThreadState HotThread::GetState() const { return m_state; }

void HotThread::Run() {
  m_thread = std::thread([&] { RunHandler(); });
  m_state = ThreadState::Running;
}
}  // namespace dlbs
