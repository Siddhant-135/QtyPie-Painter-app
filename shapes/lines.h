#pragma once
#include "base.h"

struct Line : public Shape {
  double x1;
  double y1;
  double x2;
  double y2;

  Line(double _x1, double _y1, double _x2, double _y2);

  void UpdateBoundingBox();
  void DrawObj(QPainter& p) const override;
  void MoveObj(double dx, double dy) override;
  int HitHandle(double px, double py) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void DrawHandles(QPainter& p) const override;
};
