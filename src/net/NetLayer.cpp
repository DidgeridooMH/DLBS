#pragma once

#include <net/NetLayer.hpp>

namespace dlbs {
NetLayer::~NetLayer() {}

void NetLayer::SetParent(NetLayer* parent) { m_parent = parent; }

void NetLayer::PushLayer(std::unique_ptr<NetLayer>& child) {
  if (m_child != nullptr) {
    m_child->PushLayer(child);
  } else {
    m_child = std::move(child);
  }
}
void NetLayer::OnMessage(const std::vector<uint8_t>& buffer, size_t length) {
  if (m_child != nullptr) {
    m_child->OnMessage(buffer, length);
  }
}
}  // namespace dlbs
