#pragma once

#include <functional>
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

class SvgParser {
 public:
    static SvgData parseSvgFile(const std::string& path);
 private:
    static void parseXml(const std::string& content, std::function<void(std::string, AttrMap)> Parse_Tag);
};
