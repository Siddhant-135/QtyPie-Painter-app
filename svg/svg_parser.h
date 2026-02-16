#pragma once

#include <string>
#include <unordered_map>
#include <vector>

using AttrMap = std::unordered_map<std::string, std::string>;

struct SvgTag {
  std::string name;
  AttrMap attributes;
};

struct SvgData {
  int width = 0;
  int height = 0;
  std::vector<SvgTag> elements;
};

SvgData parseSvgFile(const std::string& path);
