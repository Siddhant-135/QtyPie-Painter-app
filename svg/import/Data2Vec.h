#ifndef SVG_IMPORT_DATA2VEC_H_
#define SVG_IMPORT_DATA2VEC_H_

#include <memory>
#include <vector>

#include "../../shapes/shapes.h"
#include "../parser/Svg2Data.h"

class Data2Vec {
 public:
  static std::vector<std::unique_ptr<Shape>> ConvertToShapes(
      const SvgData& data);
};

#endif  // SVG_IMPORT_DATA2VEC_H_
