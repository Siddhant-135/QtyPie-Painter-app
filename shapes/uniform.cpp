#include "uniform.h"
#include <algorithm>
#include <cmath>

void Square::DrawObj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRect(x, y, side, side);
}

void Square::MoveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;

  if (!Shape::BoolResizeEqually(handle, dx, dy, left, top, right, bottom)) { // Checking automatically makes the changes ! movehandle only takes over and does stuff its way if the result is false.
    Shape::MoveHandle(handle, dx, dy);
    return;
  }

  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  OnBBoxChanged();
}

void Square::OnBBoxChanged() {
  const double sideNew = std::max(MIN_SIZE, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  x = bbox_x;
  y = bbox_y;
  side = sideNew;
}

void Circle::DrawObj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawEllipse(QPointF(cx, cy), radius, radius);
}

void Circle::MoveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;

  if (!Shape::BoolResizeEqually(handle, dx, dy, left, top, right, bottom)) {
    Shape::MoveHandle(handle, dx, dy);
    return;
  }

  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  OnBBoxChanged();
}

void Circle::OnBBoxChanged() {
  const double sideNew = std::max(MIN_SIZE, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  cx = bbox_x + sideNew / 2.0;
  cy = bbox_y + sideNew / 2.0;
  radius = sideNew / 2.0;
}

void Hexagon::DrawObj(QPainter& p) const {
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

void Hexagon::MoveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;
  if (!Shape::BoolResizeEqually(handle, dx, dy, left, top, right, bottom)) {
    Shape::MoveHandle(handle, dx, dy);
    return;
  }
  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  OnBBoxChanged();
}

void Hexagon::OnBBoxChanged() {
  const double sideNew = std::max(MIN_SIZE, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  cx = bbox_x + sideNew / 2.0;
  cy = bbox_y + sideNew / 2.0;
  R = sideNew / 2.0;
}
