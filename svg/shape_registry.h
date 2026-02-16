#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../shapes/shapes.h"
#include "Svg2Data.h"

using Inflater = std::function<void(Shape*, const AttrMap&)>;
using Factory = std::function<std::unique_ptr<Shape>()>;

struct ShapeTrait {
  std::string uiLabel;
  std::string svgTag;
  Factory create;
  Inflater load;
};

class ShapeRegistry {
 public:
  static const std::vector<ShapeTrait>& getRegistry();

 private:
  static void loadCircle(Shape* s, const AttrMap& a);
  static void loadRect(Shape* s, const AttrMap& a);
  static void loadLine(Shape* s, const AttrMap& a);
  static void loadSquare(Shape* s, const AttrMap& a);
  static void loadRoundedRect(Shape* s, const AttrMap& a);
};
