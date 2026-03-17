#ifndef UI_MENUS_FLOATINGMENU_H_
#define UI_MENUS_FLOATINGMENU_H_

#include <QWidget>

class QPushButton;

class FloatingMenu : public QWidget {
  Q_OBJECT
 public:
  explicit FloatingMenu(QWidget* parent = nullptr);

 signals:
  void CopyClicked();
  void CutClicked();
  void DeleteClicked();
  void PasteClicked();
};

#endif  // UI_MENUS_FLOATINGMENU_H_
