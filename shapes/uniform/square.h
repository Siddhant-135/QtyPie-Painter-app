#ifndef SHAPES_UNIFORM_SQUARE_H_
#define SHAPES_UNIFORM_SQUARE_H_

#include "../base/shape.h"

class Square : public Shape {
 public:
  double x = bbox_x;
  double y = bbox_y;
  double side = bbox_w;

  void DrawObj(QPainter& p) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void OnBBoxChanged() override;
};

#endif  // SHAPES_UNIFORM_SQUARE_H_
