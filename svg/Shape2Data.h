#pragma once

#include <QColor>
#include <string>

#include "Svg2Data.h"
#include "../shapes/shapes.h"

class Shape2Data {
 public:
	static SvgTag convert(const Shape& shape);
 private:
	static std::string colorToSvg(const QColor& c);
	static void attrsCommon(AttrMap& attrs, const Shape& s);
};
