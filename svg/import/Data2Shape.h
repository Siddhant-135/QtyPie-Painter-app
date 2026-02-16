#pragma once

#include <memory>

#include "../parser/Svg2Data.h"
#include "../../shapes/shapes.h"

class Data2Shape {
 public:
	static std::unique_ptr<Shape> Convert(const SvgTag& tag);
};
