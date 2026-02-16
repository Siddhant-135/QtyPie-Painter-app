#pragma once
#include "base.h"
#include <QString>

struct Rectangle : public Shape {
  double x = bbox_x;
  double y = bbox_y;
  double w = bbox_w;
  double h = bbox_h;

  void DrawObj(QPainter& p) const override;
  void OnBBoxChanged() override;
};

struct RoundedRectangle : public Rectangle {
  double cornerRadius = 0;

  void DrawObj(QPainter& p) const override;
  void UpdateRounding(double sliderPos);
  int HitHandle(double px, double py) const override;
  void MoveHandle(int handle, double dx, double dy) override;
  void DrawHandles(QPainter& p) const override;
  void OnBBoxChanged() override;
};

struct TextBox : public Rectangle {
  QString text_line = "Input Text";
  QString fontFamily = "Arial";
  int fontSize = 15;

  void DrawObj(QPainter& p) const override;
  void EditObj() override;
};
