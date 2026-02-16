#pragma once

#include <memory>
#include <vector>

#include "Svg2Data.h"
#include "../shapes/shapes.h"

class Data2Vec {
 public:
	static std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgData& data);
};
