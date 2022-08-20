#pragma once

#include <vector>

namespace dlbs {
class Config {
 public:
  static Config* GetInstance();

  void Load(const std::string& fileName);

  std::pair<uint32_t, uint16_t> GetEndPoint(size_t index) const;
  size_t GetNumOfEndPoints() const;

 private:
  Config();

  std::vector<std::pair<uint32_t, uint16_t>> m_endPoints;
};
}  // namespace dlbs
