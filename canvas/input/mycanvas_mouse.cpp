#include "../core/mycanvas.h"

#include <QMouseEvent>
#include <cmath>

#include "../../svg/export/Shape2Data.h"

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  const double px = event->position().x();
  const double py = event->position().y();

  if (freehand_mode_ && event->button() == Qt::LeftButton) {
    freehand_drawing_ = true;
    freehand_pts_.clear();
    freehand_pts_.emplace_back(px, py);
    return;
  }

  if (event->button() == Qt::RightButton) {
    // Right-click: select shape under cursor and show context menu
    if (selected_shape_) {
      selected_shape_->DeselectObj();
      selected_shape_ = nullptr;
    }
    for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it) {
      if (!(*it)->BboxContains(px, py)) continue;
      (*it)->EditObj();
      selected_shape_ = it->get();
      break;
    }
    update();
    // If a TextBox was right-clicked, open the text input dialog
    if (selected_shape_ && dynamic_cast<TextBox*>(selected_shape_)) {
      EditSelectedText();
      return;
    }
    ShowFloatingMenu(event->globalPosition().toPoint());
    return;
  }

  if (selected_shape_) {
    const int h = selected_shape_->HitHandle(px, py);
    if (h >= 0) {
      // Snapshot before handle drag
      for (size_t i = 0; i < shapes_.size(); ++i) {
        if (shapes_[i].get() == selected_shape_) {
          pre_drag_snapshot_ = Shape2Data::Convert(*selected_shape_);
          pre_drag_index_ = i;
          break;
        }
      }
      active_handle_ = h;
      dragging_ = true;
      last_mouse_pos_ = event->position();
      update();
      return;
    }
    selected_shape_->DeselectObj();
    selected_shape_ = nullptr;
    active_handle_ = -1;
  }

  for (auto it = shapes_.rbegin(); it != shapes_.rend(); ++it) {
    if (!(*it)->BboxContains(px, py)) continue;
    (*it)->EditObj();
    selected_shape_ = it->get();
    dragging_ = true;
    active_handle_ = -1;
    // Snapshot before body drag
    for (size_t i = 0; i < shapes_.size(); ++i) {
      if (shapes_[i].get() == selected_shape_) {
        pre_drag_snapshot_ = Shape2Data::Convert(*selected_shape_);
        pre_drag_index_ = i;
        break;
      }
    }
    last_mouse_pos_ = event->position();
    break;
  }

  update();
}

void MyCanvas::mouseMoveEvent(QMouseEvent* event) {
  if (freehand_drawing_) {
    const QPointF pos = event->position();
    if (!freehand_pts_.empty()) {
      const auto& last = freehand_pts_.back();
      if (std::abs(pos.x() - last.x()) + std::abs(pos.y() - last.y()) > 4) {
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

void MyCanvas::mouseReleaseEvent(QMouseEvent* event) {
  Q_UNUSED(event);
  if (freehand_drawing_) {
    freehand_drawing_ = false;
    if (freehand_pts_.size() >= 2) {
      auto pl = std::make_unique<Polyline>();
      pl->fillColour = Qt::transparent;
      pl->strokeColour = Qt::black;
      pl->strokeWidth = 2;
      pl->Normalise(freehand_pts_);
      AddShape(std::move(pl));
    }
    freehand_pts_.clear();
    return;
  }
  if (dragging_ && pre_drag_snapshot_ && selected_shape_) {
    undo_redo_.RecordModify(pre_drag_index_, *pre_drag_snapshot_);
  }
  pre_drag_snapshot_.reset();
  dragging_ = false;
  active_handle_ = -1;
}
