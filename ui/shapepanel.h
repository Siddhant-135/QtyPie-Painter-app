#pragma once

#include <QWidget>
#include <functional>
#include <memory>
#include "../shapes/shapes.h"

class ShapePanel : public QWidget {
  Q_OBJECT
 public:
  explicit ShapePanel(QWidget* parent = nullptr);

 signals:
  void request_shape(std::function<std::unique_ptr<Shape>()> factory);
  void sketchModeToggled(bool on);
};
