#include "shape_registry.h"

const std::vector<ShapeTrait>& ShapeRegistry::GetRegistry() {
  static const std::vector<ShapeTrait> registry = {
      {"Circle", "circle", [] { return std::make_unique<Circle>(); }, ShapeRegistry::LoadCircle},
      {"Rectangle", "rect", [] { return std::make_unique<Rectangle>(); }, ShapeRegistry::LoadRect},
      {"Square", "square", [] { return std::make_unique<Square>(); }, ShapeRegistry::LoadSquare},
      {"Hexagon", "polygon", [] { return std::make_unique<Hexagon>(); }, ShapeRegistry::LoadHexagon},
      {"Rounded Rectangle", "rrect", [] { return std::make_unique<RoundedRectangle>(); }, ShapeRegistry::LoadRoundedRect},
      {"Line", "line", [] { return std::make_unique<Line>(100, 100, 200, 200); }, ShapeRegistry::LoadLine},
      {"", "polyline", [] { return std::make_unique<Polyline>(); }, ShapeRegistry::LoadPolyline},
      {"TextBox", "textbox", [] {
        auto t = std::make_unique<TextBox>();
        t->strokeColour = QColor(0, 0, 0);
        t->strokeWidth = 1;
        t->fontSize = 15;
        return t;
      }, ShapeRegistry::LoadTextBox},
  };
  return registry;
}
