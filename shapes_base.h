#pragma once
#include <QColor>
#include <QPainter>
#include <QPen>
#include <algorithm>
#include <cmath>

struct Shape {
  virtual ~Shape() = default;
  virtual void draw(QPainter& p) const = 0;
  // edit() is defined below — sets selected = true so paintEvent draws the bbox
  double bbox_x = 100; double bbox_y = 100; double bbox_w = 100; double bbox_h = 100;
  bool selected = false;

  // Colour, stroke specifications —> sourced from ColourPanel on creation
  QColor fillColour = QColor(30, 144, 255);
  QColor strokeColour = QColor(0, 0, 0);
  int strokeWidth = 1;

  virtual bool contains(double px, double py) const {
    return px >= bbox_x && px <= bbox_x + bbox_w && py >= bbox_y && py <= bbox_y + bbox_h;
  }

  virtual void edit() { selected = true; }
  void deselect() { selected = false; }

  virtual void move(double dx, double dy) {
    bbox_x += dx;
    bbox_y += dy;
  }

  void draw_bbox(QPainter& p) const {
    p.setPen(Qt::DotLine);
    p.setBrush(Qt::NoBrush);
    p.drawRect(bbox_x, bbox_y, bbox_w, bbox_h);
  }

  // Handle system — override in shapes that need interactive control points
  static constexpr double HANDLE_SIZE = 10.0;
  static constexpr int HANDLE_TOP = 100;
  static constexpr int HANDLE_RIGHT = 101;
  static constexpr int HANDLE_BOTTOM = 102;
  static constexpr int HANDLE_LEFT = 103;

  virtual int hitHandle(double px, double py) const {
    const double cx = bbox_x + bbox_w / 2.0;
    const double cy = bbox_y + bbox_h / 2.0;

    if (std::abs(px - cx) <= HANDLE_SIZE/2 && std::abs(py - bbox_y) <= HANDLE_SIZE/2) return HANDLE_TOP;
    if (std::abs(px - (bbox_x + bbox_w)) <= HANDLE_SIZE/2 && std::abs(py - cy) <= HANDLE_SIZE/2) return HANDLE_RIGHT;
    if (std::abs(px - cx) <= HANDLE_SIZE/2 && std::abs(py - (bbox_y + bbox_h)) <= HANDLE_SIZE/2) return HANDLE_BOTTOM;
    if (std::abs(px - bbox_x) <= HANDLE_SIZE/2 && std::abs(py - cy) <= HANDLE_SIZE/2) return HANDLE_LEFT;
    return -1;
  }
  virtual void moveHandle(int handle, double dx, double dy) {
    const double MIN_SIZE = 8.0;

    double left = bbox_x;
    double top = bbox_y;
    double right = bbox_x + bbox_w;
    double bottom = bbox_y + bbox_h;

    if (handle == HANDLE_TOP) {
      top += dy;
      top = std::min(top, bottom - MIN_SIZE);
    } else if (handle == HANDLE_RIGHT) {
      right += dx;
      right = std::max(right, left + MIN_SIZE);
    } else if (handle == HANDLE_BOTTOM) {
      bottom += dy;
      bottom = std::max(bottom, top + MIN_SIZE);
    } else if (handle == HANDLE_LEFT) {
      left += dx;
      left = std::min(left, right - MIN_SIZE);
    }

    bbox_x = left;
    bbox_y = top;
    bbox_w = right - left;
    bbox_h = bottom - top;
    onBBoxChanged();
  }
  virtual void drawHandles(QPainter& p) const {
    const double cx = bbox_x + bbox_w / 2.0;
    const double cy = bbox_y + bbox_h / 2.0;
    drawHandle(p, cx, bbox_y);
    drawHandle(p, bbox_x + bbox_w, cy);
    drawHandle(p, cx, bbox_y + bbox_h);
    drawHandle(p, bbox_x, cy);
  }

  virtual void onBBoxChanged() {}

  static void drawHandle(QPainter& p, double cx, double cy) {
    p.setPen(QPen(Qt::black, 1));
    p.setBrush(Qt::white);
    p.drawRect(cx - HANDLE_SIZE/2, cy - HANDLE_SIZE/2, HANDLE_SIZE, HANDLE_SIZE);
  }
};