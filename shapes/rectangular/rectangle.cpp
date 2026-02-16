#include "rectangle.h"

void Rectangle::DrawObj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRect(x, y, w, h);
}

void Rectangle::OnBBoxChanged() {
  x = bbox_x;
  y = bbox_y;
  w = bbox_w;
  h = bbox_h;
}
