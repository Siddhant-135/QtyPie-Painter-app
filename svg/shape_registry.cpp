#include "shape_registry.h"

const std::vector<ShapeTrait>& getRegistry() {
  static const std::vector<ShapeTrait> registry = {
      {"Circle", "circle", [] { return std::make_unique<Circle>(); }, loadCircle},
      {"Rectangle", "rect", [] { return std::make_unique<Rectangle>(); }, loadRect},
      {"Square", "square", [] { return std::make_unique<Square>(); }, loadSquare},
      {"Rounded Rectangle", "rrect", [] { return std::make_unique<RoundedRectangle>(); }, loadRoundedRect},
      {"Line", "line", [] { return std::make_unique<Line>(100, 100, 200, 200); }, loadLine},
  };
  return registry;
}
