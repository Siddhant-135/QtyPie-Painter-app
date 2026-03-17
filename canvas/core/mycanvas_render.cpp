#include <QPaintEvent>
#include <QPainter>

#include "../../config/config.h"
#include "mycanvas.h"

void MyCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  if (shapes_.empty()) return;

  QPainter p(this);
  for (const auto& s : shapes_) {
    s->DrawObj(p);
    if (s->selected) {
      s->DrawBbox(p);
      s->DrawHandles(p);
    }
  }

  // Draw in-progress freehand stroke
  if (freehand_drawing_ && freehand_pts_.size() >= 2) {
    p.setPen(
        QPen(config::kFreehandPreviewColour, config::kFreehandPreviewWidth));
    p.setBrush(Qt::NoBrush);
    QPolygonF poly;
    for (const auto& pt : freehand_pts_) poly << pt;
    p.drawPolyline(poly);
  }
}
