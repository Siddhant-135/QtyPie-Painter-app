#pragma once
#include "rectangle.h"
#include <QString>

class TextBox : public Rectangle {
 public:
  QString text_line = "Input Text";
  QString fontFamily = "Arial";
  int fontSize = 15;

  void DrawObj(QPainter& p) const override;
  void EditObj() override;
};
