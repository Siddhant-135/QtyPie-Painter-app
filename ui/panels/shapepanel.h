#ifndef UI_PANELS_SHAPEPANEL_H_
#define UI_PANELS_SHAPEPANEL_H_

#include <QWidget>
#include <functional>
#include <memory>

#include "../../shapes/shapes.h"

class ShapePanel : public QWidget {
  Q_OBJECT
 public:
  explicit ShapePanel(QWidget* parent = nullptr);

 signals:
  void RequestShape(std::function<std::unique_ptr<Shape>()> factory);
  void SketchModeToggled(bool on);
};

#endif  // UI_PANELS_SHAPEPANEL_H_
