#pragma once
#include "../base/shape.h"

class Circle : public Shape {
 public:
  double cx = bbox_x + bbox_w / 2;
  double cy = bbox_y + bbox_h / 2;
  double radius = bbox_w / 2;

  void DrawObj(QPainter& p) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void OnBBoxChanged() override;
};
