#pragma once
#include "svg_parser.h"
#include "shapes.h"
#include <memory>
#include <vector>

QColor parseColor(const std::string& colorStr);

// Convert parsed SVG data into Shape objects ready for MyCanvas
std::vector<std::unique_ptr<Shape>> toShapes(const SvgParseResult& result);
