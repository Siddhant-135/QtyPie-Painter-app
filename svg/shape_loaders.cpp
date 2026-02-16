#include "shape_registry.h"

#include <algorithm>

namespace {
int val(const AttrMap& a, const std::string& k) { return a.count(k) ? std::stoi(a.at(k)) : 0; }
}

void loadCircle(Shape* s, const AttrMap& a) {
  auto* c = static_cast<Circle*>(s);
  c->cx = val(a, "cx");
  c->cy = val(a, "cy");
  c->radius = val(a, "r");
  c->bbox_x = c->cx - c->radius;
  c->bbox_y = c->cy - c->radius;
  c->bbox_w = c->bbox_h = c->radius * 2;
}

void loadRect(Shape* s, const AttrMap& a) {
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

void loadLine(Shape* s, const AttrMap& a) {
  auto* l = static_cast<Line*>(s);
  l->x1 = val(a, "x1");
  l->y1 = val(a, "y1");
  l->x2 = val(a, "x2");
  l->y2 = val(a, "y2");
  l->updateBoundingBox();
}

void loadSquare(Shape* s, const AttrMap& a) {
  auto* sq = static_cast<Square*>(s);
  sq->x = val(a, "x");
  sq->y = val(a, "y");
  sq->side = val(a, "width");
  sq->bbox_x = sq->x;
  sq->bbox_y = sq->y;
  sq->bbox_w = sq->side;
  sq->bbox_h = sq->side;
}

void loadRoundedRect(Shape* s, const AttrMap& a) {
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
