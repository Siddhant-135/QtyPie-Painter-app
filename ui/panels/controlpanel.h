#ifndef UI_PANELS_CONTROLPANEL_H_
#define UI_PANELS_CONTROLPANEL_H_

#include <QWidget>

class ControlPanel : public QWidget {
  Q_OBJECT
 public:
  explicit ControlPanel(QWidget* parent = nullptr);

 signals:
  void RequestNew();
  void RequestOpen();
  void RequestSave();
  void RequestSaveAs();
  void RequestClose();
  void RequestUndo();
  void RequestRedo();
  void RequestClear();
};

#endif  // UI_PANELS_CONTROLPANEL_H_
