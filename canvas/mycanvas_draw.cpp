#include "mycanvas.h"

#include <QPaintEvent>
#include <QPainter>
#include <QInputDialog>
#include <QLineEdit>

#include "../shapes/rectangle.h"
#include "../svg/Shape2Data.h"

MyCanvas::MyCanvas(QWidget* parent) : QWidget(parent) {}

void MyCanvas::addshape(std::unique_ptr<Shape> s, bool recordUndo) {
  shapes.push_back(std::move(s));
  if (recordUndo) {
    SvgTag data = Shape2Data::convert(*shapes.back());
    undoRedo.recordAdd(shapes.size() - 1, data);
  }
  update();
}

void MyCanvas::removelastshape() {
  if (shapes.empty()) return;
  SvgTag data = Shape2Data::convert(*shapes.back());
  undoRedo.recordRemove(shapes.size() - 1, data);
  shapes.pop_back();
  update();
}

void MyCanvas::applyColourSpec(QColor fill, QColor stroke, int width) {
  if (!selectedShape) return;
  // Find index of selected shape
  for (size_t i = 0; i < shapes.size(); ++i) {
    if (shapes[i].get() != selectedShape) continue;
    SvgTag old = Shape2Data::convert(*selectedShape);
    selectedShape->fillColour = fill;
    selectedShape->strokeColour = stroke;
    selectedShape->strokeWidth = width;
    undoRedo.recordModify(i, old);
    break;
  }
  update();
}

void MyCanvas::applyFont(const QString& family) {
  if (!selectedShape) return;
  auto* tb = dynamic_cast<TextBox*>(selectedShape);
  if (!tb) return;
  for (size_t i = 0; i < shapes.size(); ++i) {
    if (shapes[i].get() != selectedShape) continue;
    SvgTag old = Shape2Data::convert(*selectedShape);
    tb->fontFamily = family;
    undoRedo.recordModify(i, old);
    break;
  }
  update();
}

void MyCanvas::applyFontSize(int size) {
  if (!selectedShape) return;
  auto* tb = dynamic_cast<TextBox*>(selectedShape);
  if (!tb) return;
  for (size_t i = 0; i < shapes.size(); ++i) {
    if (shapes[i].get() != selectedShape) continue;
    SvgTag old = Shape2Data::convert(*selectedShape);
    tb->fontSize = size;
    undoRedo.recordModify(i, old);
    break;
  }
  update();
}

void MyCanvas::editSelectedText() {
  if (!selectedShape) return;
  auto* tb = dynamic_cast<TextBox*>(selectedShape);
  if (!tb) return;
  bool ok = false;
  QString result = QInputDialog::getText(this, "Edit Text", "Enter text:",
                                          QLineEdit::Normal, tb->text_line, &ok);
  if (ok && !result.isEmpty()) {
    for (size_t i = 0; i < shapes.size(); ++i) {
      if (shapes[i].get() != selectedShape) continue;
      SvgTag old = Shape2Data::convert(*selectedShape);
      tb->text_line = result;
      undoRedo.recordModify(i, old);
      break;
    }
    update();
  }
}

const std::vector<std::unique_ptr<Shape>>& MyCanvas::getShapes() const { return shapes; }

void MyCanvas::setFreehandMode(bool on) {
  freehandMode = on;
  freehandDrawing = false;
  freehandPts.clear();
}

void MyCanvas::undo() {
  selectedShape = nullptr;
  undoRedo.undo(shapes);
  update();
}

void MyCanvas::redo() {
  selectedShape = nullptr;
  undoRedo.redo(shapes);
  update();
}

void MyCanvas::clearAll() {
  selectedShape = nullptr;
  shapes.clear();
  undoRedo = UndoRedoManager();
  freehandDrawing = false;
  freehandPts.clear();
  update();
}

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

  // Draw in-progress freehand stroke
  if (freehandDrawing && freehandPts.size() >= 2) {
    p.setPen(QPen(Qt::black, 2));
    p.setBrush(Qt::NoBrush);
    QPolygonF poly;
    for (const auto& pt : freehandPts) poly << pt;
    p.drawPolyline(poly);
  }
}
