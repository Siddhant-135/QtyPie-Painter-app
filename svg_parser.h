#pragma once
#include <QColor>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// Parsed output types — values are int, QColor, or std::string.
using AttrValue = std::variant<int, QColor, std::string>;

struct ParsedShape {
    std::string tag;
    std::unordered_map<std::string, AttrValue> attrs;
};

struct SvgParseResult {
    int svg_width  = 0;
    int svg_height = 0;
    std::vector<ParsedShape> shapes;
};

SvgParseResult parseSvgString(const std::string& svg_string);
SvgParseResult parseSvgFile(const std::string& filepath);
