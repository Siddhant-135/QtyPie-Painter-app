#include "square.h"
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

void Square::OnBBoxChanged() {
  const double sideNew = std::max(kMinSize, std::min(bbox_w, bbox_h));
  bbox_w = sideNew;
  bbox_h = sideNew;
  x = bbox_x;
  y = bbox_y;
  side = sideNew;
}
