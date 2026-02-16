#pragma once

#include <QWidget>

class QPushButton;

class FloatingMenu : public QWidget {
  Q_OBJECT
 public:
  explicit FloatingMenu(QWidget* parent = nullptr);

 signals:
  void copyClicked();
  void cutClicked();
  void deleteClicked();
  void pasteClicked();
};
