#pragma once

#include <QColor>
#include <string>

#include "Svg2Data.h"
#include "../shapes/shapes.h"

class Shape2Data {
 public:
	static SvgTag convert(const Shape& shape); // SvgTag is our custom class to store shape details 
 private:
	static std::string colorToSvg(const QColor& c); // handles stuff like red -> #ff0000 and rgba(255,0,0,0.5) -> #ff000080 etc. (QColor)
	static void attrsCommon(AttrMap& attrs, const Shape& s);// handles the common attributes like fill, stroke, stroke-width for all shapes. static void because it doesn't need to access any instance data, and can be reused across different shape conversions.
};
