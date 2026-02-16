#include "uniform.h"
#include <algorithm>
#include <cmath>

void Square::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRect(x, y, side, side);
}

void Square::moveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;

  if (!Shape::bool_resize_equally(handle, dx, dy, left, top, right, bottom)) { // Checking automatically makes the changes ! movehandle only takes over and does stuff its way if the result is false.
    Shape::moveHandle(handle, dx, dy);
    return;
  }

  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  onBBoxChanged();
}

void Square::onBBoxChanged() {
  const double sideNew = std::max(MIN_SIZE, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  x = bbox_x;
  y = bbox_y;
  side = sideNew;
}

void Circle::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawEllipse(QPointF(cx, cy), radius, radius);
}

void Circle::moveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;

  if (!Shape::bool_resize_equally(handle, dx, dy, left, top, right, bottom)) {
    Shape::moveHandle(handle, dx, dy);
    return;
  }

  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  onBBoxChanged();
}

void Circle::onBBoxChanged() {
  const double sideNew = std::max(MIN_SIZE, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  cx = bbox_x + sideNew / 2.0;
  cy = bbox_y + sideNew / 2.0;
  radius = sideNew / 2.0;
}

void Hexagon::draw_obj(QPainter& p) const {
  constexpr double PI = 3.14159265358979323846;
  QPolygonF poly;
  for (int i = 0; i < 6; ++i) {
    const double ang = PI + i * (PI / 3.0);
    poly << QPointF(cx + R * std::cos(ang), cy + R * std::sin(ang));
  }
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawPolygon(poly);
}

void Hexagon::moveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;
  if (!Shape::bool_resize_equally(handle, dx, dy, left, top, right, bottom)) {
    Shape::moveHandle(handle, dx, dy);
    return;
  }
  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  onBBoxChanged();
}

void Hexagon::onBBoxChanged() {
  const double sideNew = std::max(MIN_SIZE, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  cx = bbox_x + sideNew / 2.0;
  cy = bbox_y + sideNew / 2.0;
  R = sideNew / 2.0;
}
