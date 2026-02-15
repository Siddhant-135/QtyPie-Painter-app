#pragma once
#include <QColor>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using AttributeValue = std::variant<int, QColor, std::string>;

struct RawShapeData {
    std::string tagName;
    std::unordered_map<std::string, AttributeValue> attributes;
};

struct SvgDocument {
    int width = 0;
    int height = 0;
    std::vector<RawShapeData> shapes;
};

SvgDocument parseSvgString(const std::string& xmlContent);
SvgDocument parseSvgFile(const std::string& filePath);