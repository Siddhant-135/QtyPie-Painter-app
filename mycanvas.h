#pragma once
#include <QWidget>
#include <memory>
#include <vector>
#include "shapes.h"

class QPaintEvent;
class QMouseEvent;

class MyCanvas : public QWidget {
  Q_OBJECT
public:
  explicit MyCanvas(QWidget* parent = nullptr);
  void addshape(std::unique_ptr<Shape> s);
  void removelastshape();
  void applyColourSpec(QColor fill, QColor stroke, int width);

protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;

private:
  std::vector<std::unique_ptr<Shape>> shapes;
  Shape* selectedShape = nullptr;
};