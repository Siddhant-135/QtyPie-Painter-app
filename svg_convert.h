#pragma once
#include "svg_parser.h"
#include "shapes.h"
#include <memory>
#include <vector>

QColor stringToColor(const std::string& colorName);
std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgDocument& svgData);