#ifndef SHAPES_UNIFORM_HEXAGON_H_
#define SHAPES_UNIFORM_HEXAGON_H_

#include "../base/shape.h"

class Hexagon : public Shape {
 public:
  double cx = bbox_x + bbox_w / 2;
  double cy = bbox_y + bbox_h / 2;
  double R = bbox_w / 2;

  void DrawObj(QPainter& p) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void OnBBoxChanged() override;
};

#endif  // SHAPES_UNIFORM_HEXAGON_H_
