#include "mycanvas.h"

#include <QMouseEvent>
#include <cmath>

#include "../svg/Shape2Data.h"

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  const double px = event->position().x();
  const double py = event->position().y();

  if (freehandMode && event->button() == Qt::LeftButton) {
    freehandDrawing = true;
    freehandPts.clear();
    freehandPts.emplace_back(px, py);
    return;
  }

  if (event->button() == Qt::RightButton) {
    // Right-click: select shape under cursor and show context menu
    if (selectedShape) {
      selectedShape->deselect_obj();
      selectedShape = nullptr;
    }
    for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
      if (!(*it)->bbox_contains(px, py)) continue;
      (*it)->edit_obj();
      selectedShape = it->get();
      break;
    }
    update();
    // If a TextBox was right-clicked, open the text input dialog
    if (selectedShape && dynamic_cast<TextBox*>(selectedShape)) {
      editSelectedText();
      return;
    }
    showFloatingMenu(event->globalPosition().toPoint());
    return;
  }

  if (selectedShape) {
    const int h = selectedShape->hitHandle(px, py);
    if (h >= 0) {
      // Snapshot before handle drag
      for (size_t i = 0; i < shapes.size(); ++i) {
        if (shapes[i].get() == selectedShape) {
          preDragSnapshot = Shape2Data::convert(*selectedShape);
          preDragIndex = i;
          break;
        }
      }
      activeHandle = h;
      dragging = true;
      lastMousePos = event->position();
      update();
      return;
    }
    selectedShape->deselect_obj();
    selectedShape = nullptr;
    activeHandle = -1;
  }

  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    if (!(*it)->bbox_contains(px, py)) continue;
    (*it)->edit_obj();
    selectedShape = it->get();
    dragging = true;
    activeHandle = -1;
    // Snapshot before body drag
    for (size_t i = 0; i < shapes.size(); ++i) {
      if (shapes[i].get() == selectedShape) {
        preDragSnapshot = Shape2Data::convert(*selectedShape);
        preDragIndex = i;
        break;
      }
    }
    lastMousePos = event->position();
    break;
  }

  update();
}

void MyCanvas::mouseMoveEvent(QMouseEvent* event) {
  if (freehandDrawing) {
    const QPointF pos = event->position();
    if (!freehandPts.empty()) {
      const auto& last = freehandPts.back();
      if (std::abs(pos.x() - last.x()) + std::abs(pos.y() - last.y()) > 4) {
        freehandPts.push_back(pos);
        update();
      }
    }
    return;
  }
  if (!dragging || !selectedShape) return;

  const QPointF pos = event->position();
  const double dx = pos.x() - lastMousePos.x();
  const double dy = pos.y() - lastMousePos.y();

  if (activeHandle >= 0) {
    selectedShape->moveHandle(activeHandle, dx, dy);
  } else {
    selectedShape->move_obj(dx, dy);
  }

  lastMousePos = pos;
  update();
}

void MyCanvas::mouseReleaseEvent(QMouseEvent* event) {
  Q_UNUSED(event);
  if (freehandDrawing) {
    freehandDrawing = false;
    if (freehandPts.size() >= 2) {
      auto pl = std::make_unique<Polyline>();
      pl->fillColour = Qt::transparent;
      pl->strokeColour = Qt::black;
      pl->strokeWidth = 2;
      pl->normalise(freehandPts);
      addshape(std::move(pl));
    }
    freehandPts.clear();
    return;
  }
  if (dragging && preDragSnapshot && selectedShape) {
    undoRedo.recordModify(preDragIndex, *preDragSnapshot);
  }
  preDragSnapshot.reset();
  dragging = false;
  activeHandle = -1;
}
