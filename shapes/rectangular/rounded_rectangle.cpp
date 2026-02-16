#include "rounded_rectangle.h"

#include <algorithm>
#include <cmath>

void RoundedRectangle::DrawObj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRoundedRect(x, y, w, h, cornerRadius, cornerRadius);
}

void RoundedRectangle::UpdateRounding(double sliderPos) {
  const double shortSide = std::min(w, h);
  cornerRadius = shortSide * std::min(0.5, sliderPos);
}

int RoundedRectangle::HitHandle(double px, double py) const {
  const double hx = x + cornerRadius;
  const double hy = y;
  if (std::abs(px - hx) <= kHandleSize / 2 && std::abs(py - hy) <= kHandleSize / 2) return 0;
  return Shape::HitHandle(px, py);
}

void RoundedRectangle::MoveHandle(int handle, double dx, double dy) {
  if (handle == 0) {
    const double maxR = std::min(w, h) / 2.0;
    cornerRadius = std::max(0.0, std::min(cornerRadius + dx, maxR));
    return;
  }

  Shape::MoveHandle(handle, dx, dy);
  cornerRadius = std::max(0.0, std::min(cornerRadius, std::min(w, h) / 2.0));
}

void RoundedRectangle::DrawHandles(QPainter& p) const {
  Shape::DrawHandles(p);
  DrawHandle(p, x + cornerRadius, y);
}

void RoundedRectangle::OnBBoxChanged() {
  Rectangle::OnBBoxChanged();
  cornerRadius = std::max(0.0, std::min(cornerRadius, std::min(w, h) / 2.0));
}
