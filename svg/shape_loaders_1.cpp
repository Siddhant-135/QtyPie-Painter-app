#include "shape_registry.h"

#include <QPointF>
#include <cmath>
#include <sstream>
#include <vector>

namespace {
int val(const AttrMap& a, const std::string& k) {
    if (a.count(k)) return std::stoi(a.at(k));
    return 0;
}

std::vector<QPointF> parsePoints(const std::string& pointsString) {
  std::vector<QPointF> pts;
  std::stringstream ss(pointsString);
  std::string token;
  while (ss >> token) {
    const size_t comma = token.find(',');
    if (comma == std::string::npos) continue;
    const double x = std::stod(token.substr(0, comma));
    const double y = std::stod(token.substr(comma + 1));
    pts.emplace_back(x, y);
  }
  return pts;
}
}

// INITIAL FEW SHAPES
void ShapeRegistry::loadCircle(Shape* s, const AttrMap& a) {
  auto* c = static_cast<Circle*>(s);
  c->cx = val(a, "cx");
  c->cy = val(a, "cy");
  c->radius = val(a, "r");
  c->bbox_x = c->cx - c->radius;
  c->bbox_y = c->cy - c->radius;
  c->bbox_w = c->bbox_h = c->radius * 2;
}

void ShapeRegistry::loadRect(Shape* s, const AttrMap& a) {
  auto* r = static_cast<Rectangle*>(s);
  r->x = val(a, "x");
  r->y = val(a, "y");
  r->w = val(a, "width");
  r->h = val(a, "height");
  r->bbox_x = r->x;
  r->bbox_y = r->y;
  r->bbox_w = r->w;
  r->bbox_h = r->h;
}

void ShapeRegistry::loadSquare(Shape* s, const AttrMap& a) {
  auto* sq = static_cast<Square*>(s);
  sq->x = val(a, "x");
  sq->y = val(a, "y");
  sq->side = val(a, "width");
  sq->bbox_x = sq->x;
  sq->bbox_y = sq->y;
  sq->bbox_w = sq->side;
  sq->bbox_h = sq->side;
}

void ShapeRegistry::loadRoundedRect(Shape* s, const AttrMap& a) {
  auto* r = static_cast<RoundedRectangle*>(s);
  r->x = val(a, "x");
  r->y = val(a, "y");
  r->w = val(a, "width");
  r->h = val(a, "height");
  r->cornerRadius = val(a, "rx");
  r->bbox_x = r->x;
  r->bbox_y = r->y;
  r->bbox_w = r->w;
  r->bbox_h = r->h;
}

void ShapeRegistry::loadLine(Shape* s, const AttrMap& a) {
  auto* l = static_cast<Line*>(s);
  l->x1 = val(a, "x1");
  l->y1 = val(a, "y1");
  l->x2 = val(a, "x2");
  l->y2 = val(a, "y2");
  l->updateBoundingBox();
}

void ShapeRegistry::loadPolyline(Shape* s, const AttrMap& a) {
  auto* pl = static_cast<Polyline*>(s);
  if (!a.count("points")) return;
  const auto pts = parsePoints(a.at("points"));
  std::vector<QPointF> raw;
  raw.reserve(pts.size());
  for (const auto& p : pts) raw.push_back(p);
  pl->normalise(raw);
}

void ShapeRegistry::loadHexagon(Shape* s, const AttrMap& a) {
  auto* h = static_cast<Hexagon*>(s);
  if (!a.count("points")) return;

  const auto pts = parsePoints(a.at("points"));
  if (pts.size() < 6) return;

  const double dx = pts[1].x() - pts[0].x();
  const double dy = pts[1].y() - pts[0].y();
  h->R = std::sqrt(dx * dx + dy * dy);  // side length == circumradius

  h->cx = (pts[0].x() + pts[3].x()) / 2.0;
  h->cy = (pts[0].y() + pts[3].y()) / 2.0;

  h->bbox_x = h->cx - h->R;
  h->bbox_y = h->cy - h->R;
  h->bbox_w = 2.0 * h->R;
  h->bbox_h = 2.0 * h->R;
}
