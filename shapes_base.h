#pragma once
#include <QColor>
#include <QPainter>
#include <QPen>
#include <algorithm>
#include <cmath>

struct Shape {
  virtual ~Shape() = default;
  virtual void draw_obj(QPainter& p) const = 0;

  double bbox_x = 100; double bbox_y = 100; double bbox_w = 100; double bbox_h = 100; // DEFINING all the defaults for each shape
  bool selected = false;
  QColor fillColour = QColor(30, 144, 255);
  QColor strokeColour = QColor(0, 0, 0);
  int strokeWidth = 1;

  virtual bool bbox_contains(double px, double py) const { // Used for selection logic, checks if the point is within the bounding box. 
    return px >= bbox_x && px <= bbox_x+bbox_w && py >= bbox_y && py <= bbox_y+bbox_h;
  }

  virtual void edit_obj() {selected = true;}
  void deselect_obj() {selected = false;}
  virtual void move_obj(double dx, double dy) { bbox_x += dx; bbox_y += dy;}

  void draw_bbox(QPainter& p) const {
    p.setPen(Qt::DotLine);
    p.setBrush(Qt::NoBrush);
    p.drawRect(bbox_x, bbox_y, bbox_w, bbox_h);
  }
// HANDLES RELATED LOGIC: (ALso enforces minimum size of Shapes in general)
  static constexpr double HANDLE_SIZE = 10.0; // Fixed constant for the whole class + Shared across shapes instead of copying.
  static constexpr double MIN_SIZE = 8.0; // Minimum size for shapes to prevent disappearing, used in moveHandle logic. -> easier geometry management without flipping
  enum Handle { TOP = 1, RIGHT = 2, BOTTOM = 3, LEFT = 4};
 
  virtual int hitHandle(double px, double py) const { //HitHandle checks if the mouse click is within a certain distance of the handle positions, and returns which handle (if any) was hit. 
    const double cx = bbox_x + bbox_w / 2.0;
    const double cy = bbox_y + bbox_h / 2.0;
    double dx = std::abs(px - cx); double dy = std::abs(py - cy); double hd = HANDLE_SIZE/ 2.0;
    if (dx<=hd/2) {
        if (std::abs(py - bbox_y) <= hd/2) return Handle::TOP;
        else if (std::abs(py - (bbox_y + bbox_h)) <= hd/2) return Handle::BOTTOM;
    } else if (dy<=hd/2) {
        if (std::abs(px - (bbox_x + bbox_w)) <= hd/2) return Handle::RIGHT;
        else if (std::abs(px - bbox_x) <= hd/2) return Handle::LEFT;
    }
    return -1;
  }
  virtual void moveHandle(int handle, double dx, double dy) { //MoveHandle then adjusts the bounding box based on which handle is being dragged, while enforcing a minimum size to prevent flipping.
    double left = bbox_x, top = bbox_y, right = bbox_x + bbox_w, bottom = bbox_y + bbox_h;

    if (handle == Handle::TOP) {
      top += dy;  
      top = std::min(top, bottom - MIN_SIZE); // Got it wrong the first time: Bottom - Top - Height > MIN_SIZE -> Top < Bottom - MIN_SIZE
    } else if (handle == Handle::RIGHT) {
      right += dx;
      right = std::max(right, left + MIN_SIZE);
    } else if (handle == Handle::BOTTOM) {
      bottom += dy;
      bottom = std::max(bottom, top + MIN_SIZE);
    } else if (handle == Handle::LEFT) {
      left += dx;
      left = std::min(left, right - MIN_SIZE);
    }

    bbox_x = left; bbox_y = top; bbox_w = right - left; bbox_h = bottom - top;
    onBBoxChanged();
  }
  virtual void drawHandles(QPainter& p) const { // DrawHandles simply draws the handles at the appropriate positions around the bounding box. virtual coz extra handles for some properties like corner radius. 
    const double cx = bbox_x + bbox_w / 2.0, cy = bbox_y + bbox_h / 2.0;
    drawHandle(p, cx, bbox_y);
    drawHandle(p, bbox_x + bbox_w, cy);
    drawHandle(p, cx, bbox_y + bbox_h);
    drawHandle(p, bbox_x, cy);
  }
  virtual void onBBoxChanged() {}

  // Utility for square-like constrained resizing (used by Square/Circle)
  static bool bool_resize_equally(int handle, double dx, double dy, double& left, double& top, double& right, double& bottom) {
    const double cx = (left + right) / 2.0, cy = (top + bottom) / 2.0;

    if (handle == Handle::TOP) {
      top += dy;
      const double side = std::max(MIN_SIZE, bottom - top);
      top = bottom - side;
      left = cx - side / 2.0;
      right = cx + side / 2.0;
    } else if (handle == Handle::BOTTOM) {
      bottom += dy;
      const double side = std::max(MIN_SIZE, bottom - top);
      bottom = top + side;
      left = cx - side / 2.0;
      right = cx + side / 2.0;
    } else if (handle == Handle::LEFT) {
      left += dx;
      const double side = std::max(MIN_SIZE, right - left);
      left = right - side;
      top = cy - side / 2.0;
      bottom = cy + side / 2.0;
    } else if (handle == Handle::RIGHT) {
      right += dx;
      const double side = std::max(MIN_SIZE, right - left);
      right = left + side;
      top = cy - side / 2.0;
      bottom = cy + side / 2.0;
    } else {
      return false;
    }
    return true;
  }

  static void drawHandle(QPainter& p, double cx, double cy) {
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(Qt::white);
    p.drawRect(cx - HANDLE_SIZE/2, cy - HANDLE_SIZE/2, HANDLE_SIZE, HANDLE_SIZE);
  }
};