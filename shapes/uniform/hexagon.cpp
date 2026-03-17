#include "hexagon.h"

#include <algorithm>
#include <cmath>

void Hexagon::DrawObj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  constexpr double kPi = 3.14159265358979323846;

  QPolygonF poly;
  for (int i = 0; i < 6; ++i) {
    const double ang = kPi + i * (kPi / 3.0);
    poly << QPointF(cx + R * std::cos(ang), cy + R * std::sin(ang));
  }
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
  const double sideNew = std::max(kMinSize, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  cx = bbox_x + sideNew / 2.0;
  cy = bbox_y + sideNew / 2.0;
  R = sideNew / 2.0;
}
