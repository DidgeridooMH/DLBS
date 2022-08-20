#include "config/Config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

extern "C" {
#include <arpa/inet.h>
}

using json = nlohmann::json;

namespace dlbs {
Config* Config::GetInstance() {
  static Config config;
  return &config;
}

Config::Config() {}

void Config::Load(const std::string& fileName) {
  std::ifstream file(fileName);

  json configFile = json::parse(file);

  if (configFile.contains("endpoints")) {
    auto endpoints = configFile["endpoints"];
    for (auto& route : endpoints) {
      m_endPoints.push_back(
          {inet_addr(route["address"].get<std::string>().c_str()),
           route["port"]});
    }
  }
}

std::pair<uint32_t, uint16_t> Config::GetEndPoint(size_t index) const {
  return m_endPoints[index % m_endPoints.size()];
}

size_t Config::GetNumOfEndPoints() const { return m_endPoints.size(); }
}  // namespace dlbs
