#pragma once

#include <algorithm>
#include <cctype>
#include <net/NetUtils.hpp>
#include <string>
#include <unordered_map>

namespace dlbs::filters {

typedef std::unordered_map<std::string, std::string> HeaderOptions;

void http(NetBuffer& buffer, size_t size);

}  // namespace dlbs::filters
