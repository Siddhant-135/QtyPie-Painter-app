#include "lines.h"

#include <cmath>

Line::Line(double _x1, double _y1, double _x2, double _y2)
    : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {
  updateBoundingBox();
}

void Line::updateBoundingBox() {
  bbox_x = std::min(x1, x2) - 1;
  bbox_y = std::min(y1, y2) - 1;
  bbox_w = std::abs(x2 - x1) + 2;
  bbox_h = std::abs(y2 - y1) + 2;
}

void Line::draw_obj(QPainter& p) const {
  p.setBrush(Qt::NoBrush);
  p.setPen(QPen(strokeColour, strokeWidth));
  p.drawLine(x1, y1, x2, y2);
}

void Line::move_obj(double dx, double dy) {
  x1 += dx;
  y1 += dy;
  x2 += dx;
  y2 += dy;
  updateBoundingBox();
}

int Line::hitHandle(double px, double py) const {
  if (std::abs(px - x1) <= HANDLE_SIZE / 2 && std::abs(py - y1) <= HANDLE_SIZE / 2) return 0;
  if (std::abs(px - x2) <= HANDLE_SIZE / 2 && std::abs(py - y2) <= HANDLE_SIZE / 2) return 1;
  return Shape::hitHandle(px, py);
}

void Line::moveHandle(int handle, double dx, double dy) {
  if (handle == 0) {
    x1 += dx;
    y1 += dy;
  } else if (handle == 1) {
    x2 += dx;
    y2 += dy;
  } else if (handle == Handle::LEFT) {
    if (x1 <= x2) x1 += dx;
    else x2 += dx;
  } else if (handle == Handle::RIGHT) {
    if (x1 >= x2) x1 += dx;
    else x2 += dx;
  } else if (handle == Handle::TOP) {
    if (y1 <= y2) y1 += dy;
    else y2 += dy;
  } else if (handle == Handle::BOTTOM) {
    if (y1 >= y2) y1 += dy;
    else y2 += dy;
  }
  updateBoundingBox();
}

void Line::drawHandles(QPainter& p) const {
  Shape::drawHandles(p);
  drawHandle(p, x1, y1);
  drawHandle(p, x2, y2);
}
