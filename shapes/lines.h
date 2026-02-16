#pragma once
#include "base.h"

struct Line : public Shape {
  double x1;
  double y1;
  double x2;
  double y2;

  Line(double _x1, double _y1, double _x2, double _y2);

  void updateBoundingBox();
  void draw_obj(QPainter& p) const override;
  void move_obj(double dx, double dy) override;
  int hitHandle(double px, double py) const override;
  void moveHandle(int handle, double dx, double dy) override;
  void drawHandles(QPainter& p) const override;
};
