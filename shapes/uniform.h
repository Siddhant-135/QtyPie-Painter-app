#pragma once
#include "base.h"

struct Square : public Shape {
  double x = bbox_x;
  double y = bbox_y;
  double side = bbox_w;

  void draw_obj(QPainter& p) const override;
  void moveHandle(int handle, double dx, double dy) override;
  void onBBoxChanged() override;
};

struct Circle : public Shape {
  double cx = bbox_x + bbox_w / 2;
  double cy = bbox_y + bbox_h / 2;
  double radius = bbox_w / 2;

  void draw_obj(QPainter& p) const override;
  void moveHandle(int handle, double dx, double dy) override;
  void onBBoxChanged() override;
};

struct Hexagon : public Shape {
  double cx = bbox_x + bbox_w / 2;
  double cy = bbox_y + bbox_h / 2;
  double R = bbox_w / 2; 

  void draw_obj(QPainter& p) const override;
  void moveHandle(int handle, double dx, double dy) override;
  void onBBoxChanged() override;
};
