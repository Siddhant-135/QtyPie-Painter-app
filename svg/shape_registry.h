#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../shapes/shapes.h"
#include "svg_parser.h"

using Inflater = std::function<void(Shape*, const AttrMap&)>;
using Factory = std::function<std::unique_ptr<Shape>()>;

struct ShapeTrait {
  std::string uiLabel;
  std::string svgTag;
  Factory create;
  Inflater load;
};

void loadCircle(Shape* s, const AttrMap& a);
void loadRect(Shape* s, const AttrMap& a);
void loadLine(Shape* s, const AttrMap& a);
void loadSquare(Shape* s, const AttrMap& a);
void loadRoundedRect(Shape* s, const AttrMap& a);

const std::vector<ShapeTrait>& getRegistry();
