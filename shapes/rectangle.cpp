#include "rectangle.h"

#include <algorithm>
#include <cmath>

void Rectangle::draw_obj(QPainter& p) const {
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  p.drawRect(x, y, w, h);
}

void Rectangle::onBBoxChanged() {
  x = bbox_x;
  y = bbox_y;
  w = bbox_w;
  h = bbox_h;
}

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

#include <QFont>
#include <QFontMetrics>
#include <QInputDialog>
#include <QLineEdit>

void TextBox::draw_obj(QPainter& p) const {
  // Transparent background, no fill rectangle drawn
  p.setPen(Qt::NoPen);
  p.setBrush(Qt::transparent);
  p.drawRect(QRectF(x, y, w, h));

  // strokeColour = text colour, fontSize = text size
  QFont font(fontFamily, fontSize);
  p.setFont(font);
  p.setPen(strokeColour);
  p.drawText(QRectF(x + 4, y, w - 8, h), Qt::AlignLeft | Qt::AlignVCenter, text_line);
}

void TextBox::edit_obj() {
  // Just select — text editing happens via right-click in canvas
  selected = true;
}
