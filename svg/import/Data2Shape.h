#ifndef SVG_IMPORT_DATA2SHAPE_H_
#define SVG_IMPORT_DATA2SHAPE_H_

#include <memory>

#include "../../shapes/shapes.h"
#include "../parser/Svg2Data.h"

class Data2Shape {
 public:
  static std::unique_ptr<Shape> Convert(const SvgTag& tag);
};

#endif  // SVG_IMPORT_DATA2SHAPE_H_
