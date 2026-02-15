#pragma once
#include "shapes_base.h"

struct Rectangle : public Shape {
  double x = bbox_x; double y = bbox_y; double w = bbox_w; double h = bbox_h;
  void draw(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawRect(x, y, w, h);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    x += dx; y += dy;
  }
  void onBBoxChanged() override {
    x = bbox_x;
    y = bbox_y;
    w = bbox_w;
    h = bbox_h;
  }
};
struct Square : public Shape {
  double x = bbox_x; double y = bbox_y; double side = bbox_w;
  void draw(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawRect(x, y, side, side);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    x += dx; y += dy;
  }
  void moveHandle(int handle, double dx, double dy) override {
    const double MIN_SIZE = 8.0;
    double left = bbox_x;
    double top = bbox_y;
    double right = bbox_x + bbox_w;
    double bottom = bbox_y + bbox_h;
    const double cx = (left + right) / 2.0;
    const double cy = (top + bottom) / 2.0;

    if (handle == HANDLE_TOP) {
      top += dy;
      double sideNew = std::max(MIN_SIZE, bottom - top);
      top = bottom - sideNew;
      left = cx - sideNew / 2.0;
      right = cx + sideNew / 2.0;
    } else if (handle == HANDLE_BOTTOM) {
      bottom += dy;
      double sideNew = std::max(MIN_SIZE, bottom - top);
      bottom = top + sideNew;
      left = cx - sideNew / 2.0;
      right = cx + sideNew / 2.0;
    } else if (handle == HANDLE_LEFT) {
      left += dx;
      double sideNew = std::max(MIN_SIZE, right - left);
      left = right - sideNew;
      top = cy - sideNew / 2.0;
      bottom = cy + sideNew / 2.0;
    } else if (handle == HANDLE_RIGHT) {
      right += dx;
      double sideNew = std::max(MIN_SIZE, right - left);
      right = left + sideNew;
      top = cy - sideNew / 2.0;
      bottom = cy + sideNew / 2.0;
    } else {
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
  void draw(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawEllipse(QPointF(cx, cy), radius, radius);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    cx += dx; cy += dy;
  }
  void moveHandle(int handle, double dx, double dy) override {
    const double MIN_SIZE = 8.0;
    double left = bbox_x;
    double top = bbox_y;
    double right = bbox_x + bbox_w;
    double bottom = bbox_y + bbox_h;
    const double ccx = (left + right) / 2.0;
    const double ccy = (top + bottom) / 2.0;

    if (handle == HANDLE_TOP) {
      top += dy;
      double sideNew = std::max(MIN_SIZE, bottom - top);
      top = bottom - sideNew;
      left = ccx - sideNew / 2.0;
      right = ccx + sideNew / 2.0;
    } else if (handle == HANDLE_BOTTOM) {
      bottom += dy;
      double sideNew = std::max(MIN_SIZE, bottom - top);
      bottom = top + sideNew;
      left = ccx - sideNew / 2.0;
      right = ccx + sideNew / 2.0;
    } else if (handle == HANDLE_LEFT) {
      left += dx;
      double sideNew = std::max(MIN_SIZE, right - left);
      left = right - sideNew;
      top = ccy - sideNew / 2.0;
      bottom = ccy + sideNew / 2.0;
    } else if (handle == HANDLE_RIGHT) {
      right += dx;
      double sideNew = std::max(MIN_SIZE, right - left);
      right = left + sideNew;
      top = ccy - sideNew / 2.0;
      bottom = ccy + sideNew / 2.0;
    } else {
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
