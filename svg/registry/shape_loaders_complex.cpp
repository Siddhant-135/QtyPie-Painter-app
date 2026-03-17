#include <QPointF>
#include <cmath>
#include <sstream>
#include <vector>

#include "shape_registry.h"

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
}  // namespace

void ShapeRegistry::LoadTextBox(Shape* s, const AttrMap& a) {
  auto* t = static_cast<TextBox*>(s);
  t->x = val(a, "x");
  t->y = val(a, "y");
  t->w = val(a, "width");
  t->h = val(a, "height");
  if (a.count("text")) t->text_line = QString::fromStdString(a.at("text"));
  if (a.count("font-family"))
    t->fontFamily = QString::fromStdString(a.at("font-family"));
  if (a.count("font-size")) t->fontSize = std::stoi(a.at("font-size"));
  t->bbox_x = t->x;
  t->bbox_y = t->y;
  t->bbox_w = t->w;
  t->bbox_h = t->h;
}

void ShapeRegistry::LoadPolyline(Shape* s, const AttrMap& a) {
  auto* pl = static_cast<Polyline*>(s);
  if (!a.count("points")) return;
  const auto pts = parsePoints(a.at("points"));
  std::vector<QPointF> raw;
  raw.reserve(pts.size());
  for (const auto& p : pts) raw.push_back(p);
  pl->Normalise(raw);
}

void ShapeRegistry::LoadHexagon(Shape* s, const AttrMap& a) {
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
