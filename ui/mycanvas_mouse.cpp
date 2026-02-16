#include "mycanvas.h"

#include <QMouseEvent>

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  const double px = event->position().x();
  const double py = event->position().y();

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
    showFloatingMenu(event->globalPosition().toPoint());
    return;
  }

  if (selectedShape) {
    const int h = selectedShape->hitHandle(px, py);
    if (h >= 0) {
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
    lastMousePos = event->position();
    break;
  }

  update();
}

void MyCanvas::mouseMoveEvent(QMouseEvent* event) {
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
  dragging = false;
  activeHandle = -1;
}
