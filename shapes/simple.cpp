#include "simple.h"

#include <algorithm>

void Rectangle::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRect(x, y, w, h);
}

void Rectangle::move_obj(double dx, double dy) {
  Shape::move_obj(dx, dy);
  x += dx;
  y += dy;
}

void Rectangle::onBBoxChanged() {
  x = bbox_x;
  y = bbox_y;
  w = bbox_w;
  h = bbox_h;
}

void Square::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRect(x, y, side, side);
}

void Square::move_obj(double dx, double dy) {
  Shape::move_obj(dx, dy);
  x += dx;
  y += dy;
}

void Square::moveHandle(int handle, double dx, double dy) {
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

void Circle::move_obj(double dx, double dy) {
  Shape::move_obj(dx, dy);
  cx += dx;
  cy += dy;
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
