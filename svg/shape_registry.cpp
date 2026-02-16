#include "shape_registry.h"

const std::vector<ShapeTrait>& ShapeRegistry::getRegistry() {
  static const std::vector<ShapeTrait> registry = {
      {"Circle", "circle", [] { return std::make_unique<Circle>(); }, ShapeRegistry::loadCircle},
      {"TextBox", "textbox", [] {
        auto t = std::make_unique<TextBox>();
        t->strokeColour = QColor(0, 0, 0);
        t->strokeWidth = 1;
        t->fontSize = 15;
        return t;
      }, ShapeRegistry::loadTextBox},
      {"Rectangle", "rect", [] { return std::make_unique<Rectangle>(); }, ShapeRegistry::loadRect},
      {"Square", "square", [] { return std::make_unique<Square>(); }, ShapeRegistry::loadSquare},
        {"Hexagon", "polygon", [] { return std::make_unique<Hexagon>(); }, ShapeRegistry::loadHexagon},
      {"Rounded Rectangle", "rrect", [] { return std::make_unique<RoundedRectangle>(); }, ShapeRegistry::loadRoundedRect},
      {"Line", "line", [] { return std::make_unique<Line>(100, 100, 200, 200); }, ShapeRegistry::loadLine},
      {"", "polyline", [] { return std::make_unique<Polyline>(); }, ShapeRegistry::loadPolyline},
  };
  return registry;
}
