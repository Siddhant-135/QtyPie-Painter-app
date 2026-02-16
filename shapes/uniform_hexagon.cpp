#include "uniform.h"
#include <algorithm>
#include <cmath>

void Hexagon::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  constexpr double PI = 3.14159265358979323846;

  QPolygonF poly;
  for (int i = 0; i < 6; ++i) {
    const double ang = PI + i * (PI / 3.0);  // start at (cx - R, cy)
    poly << QPointF(cx + R * std::cos(ang), cy + R * std::sin(ang));
  }
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
