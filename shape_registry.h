#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "shapes.h" // Assumes you have your basic shapes class
#include "svg_parser.h"

using Inflater = std::function<void(Shape*, const AttrMap&)>;
using Factory = std::function<std::unique_ptr<Shape>()>;

struct ShapeTrait {
    std::string uiLabel;
    std::string svgTag;
    Factory create;
    Inflater load;
};

// Forward declarations of loaders (implemented in shape_loaders.cpp)
void loadCircle(Shape* s, const AttrMap& a);
void loadRect(Shape* s, const AttrMap& a);
void loadLine(Shape* s, const AttrMap& a);
void loadSquare(Shape* s, const AttrMap& a);
void loadRoundedRect(Shape* s, const AttrMap& a);

inline const std::vector<ShapeTrait>& getRegistry() {
    static const std::vector<ShapeTrait> registry = {
        {"Circle",            "circle", []{return std::make_unique<Circle>();},            loadCircle},
        {"Rectangle",         "rect",   []{return std::make_unique<Rectangle>();},         loadRect},
        {"Square",            "square",  []{return std::make_unique<Square>();},            loadSquare},
        {"Rounded Rectangle", "rrect",  []{return std::make_unique<RoundedRectangle>();},  loadRoundedRect},
        {"Line",              "line",   []{return std::make_unique<Line>(100,100,200,200);}, loadLine}
    };
    return registry;
}