#pragma once

#include <QColor>

#include <memory>
#include <string>
#include <vector>

#include "Svg2Data.h"
#include "../shapes/shapes.h"

class Data2Vec {
 public:
	static std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgData& data);
 private:
	static QColor parseColor(const std::string& name);
};
