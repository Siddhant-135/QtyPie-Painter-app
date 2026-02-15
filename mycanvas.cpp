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
    }
  }
}

void MyCanvas::mousePressEvent(QMouseEvent* event) {
  // Deselect previous selection
  if (selectedShape) {
    selectedShape->deselect();
    selectedShape = nullptr;
  }

  double px = event->position().x();
  double py = event->position().y();

  // Iterate in reverse so top-most (last drawn) shape is picked first
  for (auto it = shapes.rbegin(); it != shapes.rend(); ++it) {
    if ((*it)->contains(px, py)) {
      (*it)->edit();
      selectedShape = it->get();
      break;
    }
  }

  update();
}