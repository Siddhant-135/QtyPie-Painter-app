#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "../../shapes/shapes.h"
#include "../parser/Svg2Data.h"

using Inflater = std::function<void(Shape*, const AttrMap&)>;
using Factory = std::function<std::unique_ptr<Shape>()>;

struct ShapeTrait {
  std::string uiLabel; // the thing shown in button
  std::string svgTag; // the tag in the svg file, used to identify which shape to create when loading
  Factory create; // the function to create a new shape when the button is clicked. We use the Factory type for this, which is just a wrapper around a function that returns a unique_ptr<Shape>. This allows us to easily create new shapes without having to know the details of their constructors.
  Inflater load; // Standard terminology -> INFLATOR is which takes in the attributes from the SVG file and inflates (or fills) the shape's properties with those attributes. DEFLATOR on the other hand oudl take the shape and return a flattened version -> 
};

class ShapeRegistry {
 public:
  static const std::vector<ShapeTrait>& GetRegistry();

 private:
  static void LoadCircle(Shape* s, const AttrMap& a);
  static void LoadRect(Shape* s, const AttrMap& a);
  static void LoadTextBox(Shape* s, const AttrMap& a);
  static void LoadLine(Shape* s, const AttrMap& a);
  static void LoadSquare(Shape* s, const AttrMap& a);
  static void LoadRoundedRect(Shape* s, const AttrMap& a);
  static void LoadHexagon(Shape* s, const AttrMap& a);
  static void LoadPolyline(Shape* s, const AttrMap& a);
};
