#include "Svg2Data.h"

#include <cctype>
#include <fstream>

SvgData SvgParser::parseSvgFile(const std::string& path) {
  std::ifstream file(path); // take in the stream of input
  std::string content((std::istreambuf_iterator<char>(file)), {});
  SvgData doc; // the output data structure to be filled in
  SvgParser::parseXml(content, [&](std::string tag, AttrMap attrs) {
    if (tag == "svg") {
      if (attrs.count("width")) doc.width = std::stoi(attrs["width"]);
      if (attrs.count("height")) doc.height = std::stoi(attrs["height"]);
    } else {
      doc.elements.push_back({tag, attrs}); // the other things are the shapes. we just store their tag and attributes for now, and convert to Shapes later in the shape registry (coz we can reuse the same loading code that way)
    }
  });
  return doc;
}
