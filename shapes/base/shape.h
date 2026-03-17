#ifndef SHAPES_BASE_SHAPE_H_
#define SHAPES_BASE_SHAPE_H_

#include <QColor>
#include <QPainter>
#include <QPen>

#include "../../config/config.h"

class Shape {
 public:
  virtual ~Shape() = default;
  virtual void DrawObj(QPainter& p) const = 0;

  double bbox_x = config::kDefaultBboxX;
  double bbox_y = config::kDefaultBboxY;
  double bbox_w = config::kDefaultBboxW;
  double bbox_h = config::kDefaultBboxH;
  bool selected = false;

  QColor fillColour = config::kDefaultFillColour;
  QColor strokeColour = config::kDefaultStrokeColour;
  int strokeWidth = config::kDefaultStrokeWidth;

  static constexpr double kHandleSize = config::kHandleSize;
  static constexpr double kMinSize = config::kMinShapeSize;

  enum Handle { kTop = 1, kRight = 2, kBottom = 3, kLeft = 4 };

  virtual bool BboxContains(double px, double py) const;
  virtual void EditObj();
  void DeselectObj();
  virtual void MoveObj(double dx, double dy);

  void DrawBbox(QPainter& p) const;

  virtual int HitHandle(double px, double py) const;
  virtual void MoveHandle(int handle, double dx, double dy);
  virtual void DrawHandles(QPainter& p) const;
  virtual void OnBBoxChanged();

  static bool BoolResizeEqually(int handle, double dx, double dy, double& left,
                                double& top, double& right, double& bottom);
  static void DrawHandle(QPainter& p, double cx, double cy);
};

#endif  // SHAPES_BASE_SHAPE_H_
