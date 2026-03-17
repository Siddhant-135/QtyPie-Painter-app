#include "text_box.h"

#include <QFont>
#include <QFontMetrics>
#include <QInputDialog>
#include <QLineEdit>

#include "../../config/config.h"

void TextBox::DrawObj(QPainter& p) const {
  // Transparent background, no fill rectangle drawn
  p.setPen(Qt::NoPen);
  p.setBrush(Qt::transparent);
  p.drawRect(QRectF(x, y, w, h));

  // strokeColour = text colour, fontSize = text size
  QFont font(fontFamily, fontSize);
  p.setFont(font);
  p.setPen(strokeColour);
  p.drawText(QRectF(x + config::kTextPadLeft, y, w - config::kTextPadTotal, h),
             Qt::AlignLeft | Qt::AlignVCenter, text_line);
}

void TextBox::EditObj() {
  // Just select — text editing happens via right-click in canvas
  selected = true;
}
