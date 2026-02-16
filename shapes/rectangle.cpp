#include "rectangle.h"

#include <algorithm>
#include <cmath>

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
  if (std::abs(px - hx) <= HANDLE_SIZE / 2 && std::abs(py - hy) <= HANDLE_SIZE / 2) return 0;
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

#include <QFont>
#include <QFontMetrics>
#include <QInputDialog>
#include <QLineEdit>

void TextBox::DrawObj(QPainter& p) const {
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

void TextBox::EditObj() {
  // Just select — text editing happens via right-click in canvas
  selected = true;
}
