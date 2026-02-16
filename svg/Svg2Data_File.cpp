#include "Svg2Data.h"

#include <cctype>
#include <fstream>

SvgData SvgParser::parseSvgFile(const std::string& path) {
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), {});
  SvgData doc;

  SvgParser::parseXml(content, [&](std::string tag, AttrMap attrs) {
    if (tag == "svg") {
      if (attrs.count("width")) doc.width = std::stoi(attrs["width"]);
      if (attrs.count("height")) doc.height = std::stoi(attrs["height"]);
    } else {
      doc.elements.push_back({tag, attrs});
    }
  });

  return doc;
}
