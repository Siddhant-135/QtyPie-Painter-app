#pragma once

#include <QPointF>
#include <QWidget>

#include <memory>
#include <vector>

#include "../shapes/shapes.h"

class QMouseEvent;
class QPaintEvent;

class MyCanvas : public QWidget {
  Q_OBJECT
 public:
  explicit MyCanvas(QWidget* parent = nullptr);
  void addshape(std::unique_ptr<Shape> s);
  void removelastshape();
  void applyColourSpec(QColor fill, QColor stroke, int width);
  const std::vector<std::unique_ptr<Shape>>& getShapes() const;

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  std::vector<std::unique_ptr<Shape>> shapes;
  Shape* selectedShape = nullptr;
  bool dragging = false;
  int activeHandle = -1;
  QPointF lastMousePos;
};
