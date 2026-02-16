#pragma once

#include <QColor>

#include <memory>
#include <string>
#include <vector>

#include "../../shapes/shapes.h"

class Vec2Svg {
 public:
	static bool SaveShapesToSvgFile(const std::vector<std::unique_ptr<Shape>>& shapes, const std::string& filePath, int canvasWidth, int canvasHeight);
 private:
	static std::string ColorToSvg(const QColor& c);
	static std::string AttrsCommon(const Shape& s);
};
