#include "line.h"

#include <cmath>

#include "../../config/config.h"

Line::Line(double _x1, double _y1, double _x2, double _y2)
    : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
  UpdateBoundingBox();
}

void Line::UpdateBoundingBox() {
  bbox_x = std::min(x1, x2) - config::kLineBboxPad;
  bbox_y = std::min(y1, y2) - config::kLineBboxPad;
  bbox_w = std::abs(x2 - x1) + 2 * config::kLineBboxPad;
  bbox_h = std::abs(y2 - y1) + 2 * config::kLineBboxPad;
}

void Line::DrawObj(QPainter& p) const {
  p.setBrush(Qt::NoBrush);
  p.setPen(QPen(strokeColour, strokeWidth));
  p.drawLine(x1, y1, x2, y2);
}

void Line::MoveObj(double dx, double dy) {
  x1 += dx;
  y1 += dy;
  x2 += dx;
  y2 += dy;
  UpdateBoundingBox();
}

int Line::HitHandle(double px, double py) const {
  if (std::abs(px - x1) <= kHandleSize / 2 &&
      std::abs(py - y1) <= kHandleSize / 2)
    return 0;
  if (std::abs(px - x2) <= kHandleSize / 2 &&
      std::abs(py - y2) <= kHandleSize / 2)
    return 1;
  return Shape::HitHandle(px, py);
}

void Line::MoveHandle(int handle, double dx, double dy) {
  if (handle == 0) {
    x1 += dx;
    y1 += dy;
  } else if (handle == 1) {
    x2 += dx;
    y2 += dy;
  } else if (handle == Handle::kLeft) {
    if (x1 <= x2)
      x1 += dx;
    else
      x2 += dx;
  } else if (handle == Handle::kRight) {
    if (x1 >= x2)
      x1 += dx;
    else
      x2 += dx;
  } else if (handle == Handle::kTop) {
    if (y1 <= y2)
      y1 += dy;
    else
      y2 += dy;
  } else if (handle == Handle::kBottom) {
    if (y1 >= y2)
      y1 += dy;
    else
      y2 += dy;
  }
  UpdateBoundingBox();
}

void Line::DrawHandles(QPainter& p) const {
  Shape::DrawHandles(p);
  DrawHandle(p, x1, y1);
  DrawHandle(p, x2, y2);
}
