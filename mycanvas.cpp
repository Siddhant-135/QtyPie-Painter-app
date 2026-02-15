#include "mycanvas.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <memory>

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
  selectedShape->fillColour   = fill;
  selectedShape->strokeColour = stroke;
  selectedShape->strokeWidth  = width;
  update();
}

void MyCanvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);
  if (shapes.empty()) return;
  QPainter p(this);
  for (const auto& s : shapes) {
    s->draw(p);
    if (s->selected) {
      s->draw_bbox(p);
      s->drawHandles(p);
    }
  }
}

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  double px = event->position().x();
  double py = event->position().y();

  // If a shape is already selected, check its handles first
  if (selectedShape) {
    int h = selectedShape->hitHandle(px, py);
    if (h >= 0) {
      activeHandle = h;
      dragging = true;
      lastMousePos = event->position();
      update();
      return;
    }
    // Didn't hit a handle — deselect the old shape
    selectedShape->deselect();
    selectedShape = nullptr;
    activeHandle = -1;
  }

  // Iterate in reverse so top-most (last drawn) shape is picked first
  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    if ((*it)->contains(px, py)) {
      (*it)->edit();
      selectedShape = it->get();
      dragging = true;
      activeHandle = -1;
      lastMousePos = event->position();
      break;
    }
  }

  update();
}

void MyCanvas::mouseMoveEvent(QMouseEvent* event) {
  if (!dragging || !selectedShape) return;

  QPointF pos = event->position();
  double dx = pos.x() - lastMousePos.x();
  double dy = pos.y() - lastMousePos.y();

  if (activeHandle >= 0)
    selectedShape->moveHandle(activeHandle, dx, dy);
  else
    selectedShape->move(dx, dy);

  lastMousePos = pos;
  update();
}

void MyCanvas::mouseReleaseEvent(QMouseEvent* event) {
  Q_UNUSED(event);
  dragging = false;
  activeHandle = -1;
}