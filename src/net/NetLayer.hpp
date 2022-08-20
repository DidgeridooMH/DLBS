#pragma once

#include <memory>

namespace dlbs {
class NetLayer {
 public:
  virtual ~NetLayer();

  void SetParent(NetLayer* parent);

  void PushLayer(std::unique_ptr<NetLayer>& child);

  virtual void OnMessage(const std::vector<uint8_t>& buffer, size_t length);

 private:
  std::unique_ptr<NetLayer> m_child;
  NetLayer* m_parent;
};
}  // namespace dlbs
