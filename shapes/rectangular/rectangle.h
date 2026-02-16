#pragma once
#include "../base/shape.h"

class Rectangle : public Shape {
 public:
  double x = bbox_x;
  double y = bbox_y;
  double w = bbox_w;
  double h = bbox_h;

  void DrawObj(QPainter& p) const override;
  void OnBBoxChanged() override;
};
