#include <QMouseEvent>
#include <cmath>

#include "../../config/config.h"
#include "../../svg/export/Shape2Data.h"
#include "../core/mycanvas.h"

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  const double px = event->position().x();
  const double py = event->position().y();

  if (freehand_mode_ && event->button() == Qt::LeftButton) {
    freehand_drawing_ = true;
    freehand_pts_.clear();
    freehand_pts_.emplace_back(px, py);
    return;
  } // yay drawing was surprisingly easy

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
