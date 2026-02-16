#include "base.h"

#include <algorithm>
#include <cmath>

bool Shape::bbox_contains(double px, double py) const {
  return px >= bbox_x && px <= bbox_x + bbox_w && py >= bbox_y && py <= bbox_y + bbox_h;
}

void Shape::edit_obj() { selected = true; }

void Shape::deselect_obj() { selected = false; }

void Shape::move_obj(double dx, double dy) {
  bbox_x += dx;
  bbox_y += dy;
}

void Shape::draw_bbox(QPainter& p) const {
  p.setPen(Qt::DotLine);
  p.setBrush(Qt::NoBrush);
  p.drawRect(bbox_x, bbox_y, bbox_w, bbox_h);
}

int Shape::hitHandle(double px, double py) const {
  const double cx = bbox_x + bbox_w / 2.0;
  const double cy = bbox_y + bbox_h / 2.0;
  const double hd = HANDLE_SIZE / 2.0;

  if (std::abs(px - cx) <= hd / 2.0) {
    if (std::abs(py - bbox_y) <= hd / 2.0) return Handle::TOP;
    if (std::abs(py - (bbox_y + bbox_h)) <= hd / 2.0) return Handle::BOTTOM;
  } else if (std::abs(py - cy) <= hd / 2.0) {
    if (std::abs(px - (bbox_x + bbox_w)) <= hd / 2.0) return Handle::RIGHT;
    if (std::abs(px - bbox_x) <= hd / 2.0) return Handle::LEFT;
  }
  return -1;
}

void Shape::moveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;

  if (handle == Handle::TOP) {
    top = std::min(top + dy, bottom - MIN_SIZE);
  } else if (handle == Handle::RIGHT) {
    right = std::max(right + dx, left + MIN_SIZE);
  } else if (handle == Handle::BOTTOM) {
    bottom = std::max(bottom + dy, top + MIN_SIZE);
  } else if (handle == Handle::LEFT) {
    left = std::min(left + dx, right - MIN_SIZE);
  }

  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  onBBoxChanged();
}

void Shape::drawHandles(QPainter& p) const {
  const double cx = bbox_x + bbox_w / 2.0;
  const double cy = bbox_y + bbox_h / 2.0;
  drawHandle(p, cx, bbox_y);
  drawHandle(p, bbox_x + bbox_w, cy);
  drawHandle(p, cx, bbox_y + bbox_h);
  drawHandle(p, bbox_x, cy);
}

void Shape::onBBoxChanged() {}

bool Shape::bool_resize_equally(int handle, double dx, double dy,
                                double& left, double& top, double& right, double& bottom) {
  const double cx = (left + right) / 2.0;
  const double cy = (top + bottom) / 2.0;

  if (handle == Handle::TOP) {
    top += dy;
    const double side = std::max(MIN_SIZE, bottom - top);
    top = bottom - side;
    left = cx - side / 2.0;
    right = cx + side / 2.0;
  } else if (handle == Handle::BOTTOM) {
    bottom += dy;
    const double side = std::max(MIN_SIZE, bottom - top);
    bottom = top + side;
    left = cx - side / 2.0;
    right = cx + side / 2.0;
  } else if (handle == Handle::LEFT) {
    left += dx;
    const double side = std::max(MIN_SIZE, right - left);
    left = right - side;
    top = cy - side / 2.0;
    bottom = cy + side / 2.0;
  } else if (handle == Handle::RIGHT) {
    right += dx;
    const double side = std::max(MIN_SIZE, right - left);
    right = left + side;
    top = cy - side / 2.0;
    bottom = cy + side / 2.0;
  } else {
    return false;
  }
  return true;
}

void Shape::drawHandle(QPainter& p, double cx, double cy) {
  p.setPen(QPen(Qt::black, 1));
  p.setBrush(Qt::white);
  p.drawRect(cx - HANDLE_SIZE / 2, cy - HANDLE_SIZE / 2, HANDLE_SIZE, HANDLE_SIZE);
}
