#pragma once
#include <QWidget>
#include <QColor>
#include <QString>

class QPushButton; // forward declaration is always a good practice.
class QSpinBox;

class ColourRibbon : public QWidget {
  Q_OBJECT
public:
  explicit ColourRibbon(QWidget* parent = nullptr);
  void pickColour(QColor& target, const QString& header); // sets colour and emits signal
  void setStrokeWidth(int width) { m_strokeWidth = width; emit strokeWidthChanged(width); } // sets width and emits signal

  const QColor& getFillColour() const {return m_fillColour;} //inline defintions of get functions for use in shape colour setting etc.
  const QColor& getStrokeColour() const {return m_strokeColour;}
  int getStrokeWidth() const {return m_strokeWidth;}

signals:
  void colourChanged(const QColor& colour);
  void strokeWidthChanged(int width);

private:
  QPushButton* fillcBtn = nullptr; // Need to declare all buttons, compiler errored.
  QPushButton* strokecBtn = nullptr;
  QSpinBox* width_counter_btn = nullptr;
  QColor m_fillColour = QColor(30, 144, 255); //default values (m_ is standard for member variables)
  QColor m_strokeColour = QColor(0, 0, 0);
  int m_strokeWidth = 1;
};