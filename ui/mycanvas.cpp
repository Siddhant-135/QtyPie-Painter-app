#include "mycanvas.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

MyCanvas::MyCanvas(QWidget* parent) : QWidget(parent) {}

void MyCanvas::addshape(std::unique_ptr<Shape> s) {
  shapes.push_back(std::move(s));
  update();
}

void MyCanvas::removelastshape() {
  if (shapes.empty()) return;
  shapes.pop_back();
  update();
}

void MyCanvas::applyColourSpec(QColor fill, QColor stroke, int width) {
  if (!selectedShape) return;
  selectedShape->fillColour = fill;
  selectedShape->strokeColour = stroke;
  selectedShape->strokeWidth = width;
  update();
}

const std::vector<std::unique_ptr<Shape>>& MyCanvas::getShapes() const { return shapes; }

void MyCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  if (shapes.empty()) return;

  QPainter p(this);
  for (const auto& s : shapes) {
    s->draw_obj(p);
    if (s->selected) {
      s->draw_bbox(p);
      s->drawHandles(p);
    }
  }
}

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  const double px = event->position().x();
  const double py = event->position().y();

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
