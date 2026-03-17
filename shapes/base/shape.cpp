#include "shape.h"

#include <algorithm>
#include <cmath>

bool Shape::BboxContains(double px, double py) const {
  return px >= bbox_x && px <= bbox_x + bbox_w && py >= bbox_y &&
         py <= bbox_y + bbox_h;
}

void Shape::EditObj() { selected = true; }

void Shape::DeselectObj() { selected = false; }

void Shape::MoveObj(double dx, double dy) {
  bbox_x += dx;
  bbox_y += dy;
  OnBBoxChanged();
}

void Shape::DrawBbox(QPainter& p) const {
  p.setPen(Qt::DotLine);
  p.setBrush(Qt::NoBrush);
  p.drawRect(bbox_x, bbox_y, bbox_w, bbox_h);
}

int Shape::HitHandle(double px, double py) const {
  const double cx = bbox_x + bbox_w / 2.0;
  const double cy = bbox_y + bbox_h / 2.0;
  const double hd = kHandleSize / 2.0;

  if (std::abs(px - cx) <= hd / 2.0) {
    if (std::abs(py - bbox_y) <= hd / 2.0) return Handle::kTop;
    if (std::abs(py - (bbox_y + bbox_h)) <= hd / 2.0) return Handle::kBottom;
  } else if (std::abs(py - cy) <= hd / 2.0) {
    if (std::abs(px - (bbox_x + bbox_w)) <= hd / 2.0) return Handle::kRight;
    if (std::abs(px - bbox_x) <= hd / 2.0) return Handle::kLeft;
  }
  return -1;
}

void Shape::MoveHandle(int handle, double dx, double dy) {
  double left = bbox_x;
  double top = bbox_y;
  double right = bbox_x + bbox_w;
  double bottom = bbox_y + bbox_h;

  if (handle == Handle::kTop) {
    top = std::min(top + dy, bottom - kMinSize);
  } else if (handle == Handle::kRight) {
    right = std::max(right + dx, left + kMinSize);
  } else if (handle == Handle::kBottom) {
    bottom = std::max(bottom + dy, top + kMinSize);
  } else if (handle == Handle::kLeft) {
    left = std::min(left + dx, right - kMinSize);
  }

  bbox_x = left;
  bbox_y = top;
  bbox_w = right - left;
  bbox_h = bottom - top;
  OnBBoxChanged();
}

void Shape::DrawHandles(QPainter& p) const {
  const double cx = bbox_x + bbox_w / 2.0;
  const double cy = bbox_y + bbox_h / 2.0;
  DrawHandle(p, cx, bbox_y);
  DrawHandle(p, bbox_x + bbox_w, cy);
  DrawHandle(p, cx, bbox_y + bbox_h);
  DrawHandle(p, bbox_x, cy);
}

void Shape::OnBBoxChanged() {}

void Shape::DrawHandle(QPainter& p, double cx, double cy) {
  p.setPen(QPen(Qt::black, 1));
  p.setBrush(Qt::white);
  p.drawRect(cx - kHandleSize / 2, cy - kHandleSize / 2, kHandleSize,
             kHandleSize);
}
