#include "Shape2Data.h"

#include <QString>

std::string Shape2Data::colorToSvg(const QColor& c) {
  if (!c.isValid()) return "none";
  return c.name(QColor::HexRgb).toStdString();
}

void Shape2Data::attrsCommon(AttrMap& attrs, const Shape& s) {
  attrs["fill"] = colorToSvg(s.fillColour);
  attrs["stroke"] = colorToSvg(s.strokeColour);
  attrs["stroke-width"] = std::to_string(s.strokeWidth);
}

SvgTag Shape2Data::convert(const Shape& shape) {
  SvgTag tag;
  attrsCommon(tag.attributes, shape);

  if (const auto* rr = dynamic_cast<const RoundedRectangle*>(&shape)) {
    tag.name = "rrect";
    tag.attributes["x"] = std::to_string(rr->x);
    tag.attributes["y"] = std::to_string(rr->y);
    tag.attributes["width"] = std::to_string(rr->w);
    tag.attributes["height"] = std::to_string(rr->h);
    tag.attributes["rx"] = std::to_string(rr->cornerRadius);
  } else if (const auto* r = dynamic_cast<const Rectangle*>(&shape)) {
    tag.name = "rect";
    tag.attributes["x"] = std::to_string(r->x);
    tag.attributes["y"] = std::to_string(r->y);
    tag.attributes["width"] = std::to_string(r->w);
    tag.attributes["height"] = std::to_string(r->h);
  } else if (const auto* sq = dynamic_cast<const Square*>(&shape)) {
    tag.name = "square";
    tag.attributes["x"] = std::to_string(sq->x);
    tag.attributes["y"] = std::to_string(sq->y);
    tag.attributes["width"] = std::to_string(sq->side);
  } else if (const auto* c = dynamic_cast<const Circle*>(&shape)) {
    tag.name = "circle";
    tag.attributes["cx"] = std::to_string(c->cx);
    tag.attributes["cy"] = std::to_string(c->cy);
    tag.attributes["r"] = std::to_string(c->radius);
  } else if (const auto* l = dynamic_cast<const Line*>(&shape)) {
    tag.name = "line";
    tag.attributes["x1"] = std::to_string(l->x1);
    tag.attributes["y1"] = std::to_string(l->y1);
    tag.attributes["x2"] = std::to_string(l->x2);
    tag.attributes["y2"] = std::to_string(l->y2);
  }

  return tag;
}
