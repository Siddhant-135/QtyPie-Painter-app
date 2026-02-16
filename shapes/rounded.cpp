#include "rounded.h"

#include <algorithm>
#include <cmath>

void RoundedRectangle::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRoundedRect(x, y, w, h, cornerRadius, cornerRadius);
}

void RoundedRectangle::updateRounding(double sliderPos) {
  const double shortSide = std::min(w, h);
  cornerRadius = shortSide * std::min(0.5, sliderPos);
}

int RoundedRectangle::hitHandle(double px, double py) const {
  const double hx = x + cornerRadius;
  const double hy = y;
  if (std::abs(px - hx) <= HANDLE_SIZE / 2 && std::abs(py - hy) <= HANDLE_SIZE / 2) return 0;
  return Shape::hitHandle(px, py);
}

void RoundedRectangle::moveHandle(int handle, double dx, double dy) {
  if (handle == 0) {
    const double maxR = std::min(w, h) / 2.0;
    cornerRadius = std::max(0.0, std::min(cornerRadius + dx, maxR));
    return;
  }

  Shape::moveHandle(handle, dx, dy);
  cornerRadius = std::max(0.0, std::min(cornerRadius, std::min(w, h) / 2.0));
}

void RoundedRectangle::drawHandles(QPainter& p) const {
  Shape::drawHandles(p);
  drawHandle(p, x + cornerRadius, y);
}

void RoundedRectangle::onBBoxChanged() {
  Rectangle::onBBoxChanged();
  cornerRadius = std::max(0.0, std::min(cornerRadius, std::min(w, h) / 2.0));
}
