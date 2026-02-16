#pragma once
#include "shapes_base.h"

struct Square : public Shape {
  double x = bbox_x; double y = bbox_y; double side = bbox_w;
  void draw_obj(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawRect(x, y, side, side);
  }
  void move_obj(double dx, double dy) override {
    Shape::move_obj(dx, dy);
    x += dx; y += dy;
  }
  void moveHandle(int handle, double dx, double dy) override {
    double left = bbox_x, top = bbox_y, right = bbox_x + bbox_w, bottom = bbox_y + bbox_h;
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
  void onBBoxChanged() override {
    const double sideNew = std::max(8.0, std::min(bbox_w, bbox_h));
    bbox_w = sideNew;
    bbox_h = sideNew;
    x = bbox_x;
    y = bbox_y;
    side = sideNew;
  }
};
struct Circle : public Shape {
  double cx = (bbox_x + bbox_w/2); double cy = (bbox_y + bbox_h/2); double radius = bbox_w / 2;
  void draw_obj(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawEllipse(QPointF(cx, cy), radius, radius);
  }
  void move_obj(double dx, double dy) override {
    Shape::move_obj(dx, dy);
    cx += dx; cy += dy;
  }
  void moveHandle(int handle, double dx, double dy) override {
    double left = bbox_x, top = bbox_y, right = bbox_x + bbox_w, bottom = bbox_y + bbox_h;
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
  void onBBoxChanged() override {
    const double sideNew = std::max(8.0, std::min(bbox_w, bbox_h));
    bbox_w = sideNew;
    bbox_h = sideNew;
    cx = bbox_x + sideNew / 2.0;
    cy = bbox_y + sideNew / 2.0;
    radius = sideNew / 2.0;
  }
};
