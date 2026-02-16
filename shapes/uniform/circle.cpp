#include "circle.h"
#include <algorithm>
#include <cmath>

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
  const double sideNew = std::max(kMinSize, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  cx = bbox_x + sideNew / 2.0;
  cy = bbox_y + sideNew / 2.0;
  radius = sideNew / 2.0;
}
