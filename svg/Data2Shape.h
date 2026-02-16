#pragma once

#include <memory>

#include "Svg2Data.h"
#include "../shapes/shapes.h"

class Data2Shape {
 public:
	static std::unique_ptr<Shape> convert(const SvgTag& tag);
};
