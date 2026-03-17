#ifndef SHAPES_RECTANGULAR_TEXT_BOX_H_
#define SHAPES_RECTANGULAR_TEXT_BOX_H_

#include <QString>

#include "rectangle.h"

class TextBox : public Rectangle {
 public:
  QString text_line = config::kDefaultText;
  QString fontFamily = config::kDefaultFontFamily;
  int fontSize = config::kDefaultFontSize;

  void DrawObj(QPainter& p) const override;
  void EditObj() override;
};

#endif  // SHAPES_RECTANGULAR_TEXT_BOX_H_
