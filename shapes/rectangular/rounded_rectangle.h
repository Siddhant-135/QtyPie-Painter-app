#pragma once
#include "rectangle.h"

class RoundedRectangle : public Rectangle {
 public:
  double cornerRadius = 0;

  void DrawObj(QPainter& p) const override;
  void UpdateRounding(double sliderPos);
  int HitHandle(double px, double py) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void DrawHandles(QPainter& p) const override;
  void OnBBoxChanged() override;
};
