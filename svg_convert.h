#pragma once
#include <vector>
#include <memory>
#include "shapes.h"
#include "svg_parser.h"

std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgData& data);