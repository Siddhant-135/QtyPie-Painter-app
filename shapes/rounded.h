#pragma once

#include "simple.h"

struct RoundedRectangle : public Rectangle {
  double cornerRadius = 0;

  void draw_obj(QPainter& p) const override;
  void updateRounding(double sliderPos);
  int hitHandle(double px, double py) const override;
  void moveHandle(int handle, double dx, double dy) override;
  void drawHandles(QPainter& p) const override;
  void onBBoxChanged() override;
};
