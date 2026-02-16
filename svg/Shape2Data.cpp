#include "Shape2Data.h"

#include <QString>
#include <cmath>
#include <sstream>

std::string Shape2Data::ColorToSvg(const QColor& c) {
  if (!c.isValid()) return "none";
  return c.name(QColor::HexRgb).toStdString(); // ouptut is only Hexcodes, we dont convert to red and all.
}

void Shape2Data::AttrsCommon(AttrMap& attrs, const Shape& s) {
  attrs["fill"] = ColorToSvg(s.fillColour);
  attrs["stroke"] = ColorToSvg(s.strokeColour);
  attrs["stroke-width"] = std::to_string(s.strokeWidth);
}

SvgTag Shape2Data::Convert(const Shape& shape) {
  SvgTag tag;
  AttrsCommon(tag.attributes, shape);
  constexpr double PI = 3.14159265358979323846;

  if (const auto* tb = dynamic_cast<const TextBox*>(&shape)) {
    tag.name = "textbox";
    tag.attributes["x"] = std::to_string(tb->x);
    tag.attributes["y"] = std::to_string(tb->y);
    tag.attributes["width"] = std::to_string(tb->w);
    tag.attributes["height"] = std::to_string(tb->h);
    tag.attributes["text"] = tb->text_line.toStdString();
    tag.attributes["font-family"] = tb->fontFamily.toStdString();
    tag.attributes["font-size"] = std::to_string(tb->fontSize);
  } else if (const auto* rr = dynamic_cast<const RoundedRectangle*>(&shape)) {
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
  } else if (const auto* pl = dynamic_cast<const Polyline*>(&shape)) {
    tag.name = "polyline";
    std::ostringstream pts;
    for (size_t i = 0; i < pl->worldPts.size(); ++i) {
      if (i > 0) pts << ' ';
      pts << pl->worldPts[i].x() << ',' << pl->worldPts[i].y();
    }
    tag.attributes["points"] = pts.str();
  } else if (const auto* h = dynamic_cast<const Hexagon*>(&shape)) {
    tag.name = "polygon";
    std::ostringstream points;
    for (int i = 0; i < 6; ++i) {
      const double ang = PI + i * (PI / 3.0);  // start at (cx - R, cy)
      const double x = h->cx + h->R * std::cos(ang);
      const double y = h->cy + h->R * std::sin(ang);
      if (i > 0) points << ' ';
      points << x << ',' << y;
    }
    tag.attributes["points"] = points.str();
  }

  return tag;
}
