#pragma once
#include "base.h"

struct Square : public Shape {
  double x = bbox_x;
  double y = bbox_y;
  double side = bbox_w;

  void DrawObj(QPainter& p) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void OnBBoxChanged() override;
};

struct Circle : public Shape {
  double cx = bbox_x + bbox_w / 2;
  double cy = bbox_y + bbox_h / 2;
  double radius = bbox_w / 2;

  void DrawObj(QPainter& p) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void OnBBoxChanged() override;
};

struct Hexagon : public Shape {
  double cx = bbox_x + bbox_w / 2;
  double cy = bbox_y + bbox_h / 2;
  double R = bbox_w / 2; 

  void DrawObj(QPainter& p) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void OnBBoxChanged() override;
};
