#include <QMouseEvent>
#include <cmath>

#include "../../config/config.h"
#include "../../svg/export/Shape2Data.h"
#include "../core/mycanvas.h"

void MyCanvas::mouseMoveEvent(QMouseEvent* event) {
  if (freehand_drawing_) {
    const QPointF pos = event->position();
    if (!freehand_pts_.empty()) {
      const auto& last = freehand_pts_.back();
      if (std::abs(pos.x() - last.x()) + std::abs(pos.y() - last.y()) >
          config::kFreehandSampleDist) {
        freehand_pts_.push_back(pos);
        update();
      }
    }
    return;
  }
  if (!dragging_ || !selected_shape_) return;

  const QPointF pos = event->position();
  const double dx = pos.x() - last_mouse_pos_.x();
  const double dy = pos.y() - last_mouse_pos_.y();

  if (active_handle_ >= 0) {
    selected_shape_->MoveHandle(active_handle_, dx, dy);
  } else {
    selected_shape_->MoveObj(dx, dy);
  }

  last_mouse_pos_ = pos;
  update();
}