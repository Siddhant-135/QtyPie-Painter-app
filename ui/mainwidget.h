#pragma once

#include <QString>
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
  void newFile();
  void openFile();
  void closeFile();
  void save();
  bool saveToFile(const QString& path);
  bool saveAs();

  MyCanvas* mycanvas = nullptr;
  ColourRibbon* colourPanel = nullptr;
  ControlPanel* controlPanel = nullptr;
  ShapePanel* shapePanel = nullptr;
  QString currentFilePath;
};
