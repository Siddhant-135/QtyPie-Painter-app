#include "mycanvas.h"

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
