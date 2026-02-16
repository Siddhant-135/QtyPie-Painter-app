#pragma once

#include <QWidget>

class ColourRibbon;
class ControlPanel;
class MyCanvas;
class ShapePanel;

class MainWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MainWidget(QWidget* parent = nullptr);

 private:
  MyCanvas* mycanvas = nullptr;
  ColourRibbon* colourPanel = nullptr;
  ControlPanel* controlPanel = nullptr;
  ShapePanel* shapePanel = nullptr;
};
