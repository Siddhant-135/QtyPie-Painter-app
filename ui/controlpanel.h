#pragma once

#include <QWidget>

class ControlPanel : public QWidget {
  Q_OBJECT
 public:
  explicit ControlPanel(QWidget* parent = nullptr);

 signals:
  void request_undo();
  void request_redo();
  void request_save();
  void request_clear();
};
