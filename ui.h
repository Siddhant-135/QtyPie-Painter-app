#pragma once

#include <QWidget>

class QPaintEvent;

// A tiny "canvas" widget that can optionally draw a solid rectangle.
class Canvas : public QWidget {
public:
  explicit Canvas(QWidget* parent = nullptr);

  void showRectangle();
  void clearRectangle();

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  bool m_hasRectangle = false;
};

// The main UI widget: 2 buttons + the canvas.
class AppWindow : public QWidget {
public:
  explicit AppWindow(QWidget* parent = nullptr);

private:
  Canvas* m_canvas = nullptr;
};

