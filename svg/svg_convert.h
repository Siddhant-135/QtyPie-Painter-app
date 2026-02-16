#pragma once

#include <memory>
#include <vector>

#include "svg_parser.h"
#include "../shapes/shapes.h"

std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgData& data);
