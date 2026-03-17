#ifndef UI_MAIN_MAINWIDGET_H_
#define UI_MAIN_MAINWIDGET_H_

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
  void NewFile();
  void OpenFile();
  void CloseFile();
  void Save();
  bool SaveToFile(const QString& path);
  bool SaveAs();

  MyCanvas* canvas_ = nullptr;
  ColourRibbon* colour_panel_ = nullptr;
  ControlPanel* control_panel_ = nullptr;
  ShapePanel* shape_panel_ = nullptr;
  QString current_file_path_;
};

#endif  // UI_MAIN_MAINWIDGET_H_
