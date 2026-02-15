#pragma once
#include <QColor>
#include <QPainter>
#include <QPen>

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

  bool contains(double px, double py) const {
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
};

struct Rectangle : public Shape {
  double x = bbox_x; double y = bbox_y; double w = bbox_w; double h = bbox_h;
  void draw(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawRect(x, y, w, h);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    x += dx; y += dy;
  }
};
struct Square : public Shape {
  double x = bbox_x; double y = bbox_y; double side = bbox_w;
  void draw(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawRect(x, y, side, side);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    x += dx; y += dy;
  }
};
struct Circle : public Shape {
  double cx = (bbox_x + bbox_w/2); double cy = (bbox_y + bbox_h/2); double radius = bbox_w / 2;
  void draw(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawEllipse(QPointF(cx, cy), radius, radius);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    cx += dx; cy += dy;
  }
};

struct Line : public Shape {
  double x1 = bbox_x; double y1 = bbox_y; double x2 = bbox_x + bbox_w; double y2 = bbox_y + bbox_h;
  void draw(QPainter& p) const override {
    p.setBrush(Qt::NoBrush);
    p.setPen(QPen(strokeColour, strokeWidth));
    p.drawLine(x1, y1, x2, y2);
  }
  void move(double dx, double dy) override {
    Shape::move(dx, dy);
    x1 += dx; y1 += dy;
    x2 += dx; y2 += dy;
  }
};