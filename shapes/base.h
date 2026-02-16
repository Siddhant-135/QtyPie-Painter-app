#pragma once
#include <QColor>
#include <QPainter>
#include <QPen>

struct Shape {
  virtual ~Shape() = default;
  virtual void DrawObj(QPainter& p) const = 0;

  double bbox_x = 100;
  double bbox_y = 100;
  double bbox_w = 100;
  double bbox_h = 100;
  bool selected = false;

  QColor fillColour = QColor(30, 144, 255);
  QColor strokeColour = QColor(0, 0, 0);
  int strokeWidth = 1;

  static constexpr double HANDLE_SIZE = 10.0;
  static constexpr double MIN_SIZE = 8.0;

  enum Handle { TOP = 1, RIGHT = 2, BOTTOM = 3, LEFT = 4 };

  virtual bool BboxContains(double px, double py) const;
  virtual void EditObj();
  void DeselectObj();
  virtual void MoveObj(double dx, double dy);

  void DrawBbox(QPainter& p) const;

  virtual int HitHandle(double px, double py) const;
  virtual void MoveHandle(int handle, double dx, double dy);
  virtual void DrawHandles(QPainter& p) const;
  virtual void OnBBoxChanged();

  static bool BoolResizeEqually(int handle, double dx, double dy,double& left, double& top, double& right, double& bottom);
  static void DrawHandle(QPainter& p, double cx, double cy);
};
