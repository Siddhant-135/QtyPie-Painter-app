#include <QMouseEvent>
#include <cmath>

#include "../../config/config.h"
#include "../../svg/export/Shape2Data.h"
#include "../core/mycanvas.h"

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