#pragma once

#include <QColor>
#include <QString>
#include <QWidget>

class QPushButton;
class QSpinBox;

class ColourRibbon : public QWidget {
  Q_OBJECT
 public:
  explicit ColourRibbon(QWidget* parent = nullptr);

  void pickColour(QColor& target, const QString& header);
  void setStrokeWidth(int width);

  const QColor& getFillColour() const { return m_fillColour; }
  const QColor& getStrokeColour() const { return m_strokeColour; }
  int getStrokeWidth() const { return m_strokeWidth; }

 signals:
  void colourChanged(const QColor& colour);
  void strokeWidthChanged(int width);

 private:
  QPushButton* fillcBtn = nullptr;
  QPushButton* strokecBtn = nullptr;
  QSpinBox* width_counter_btn = nullptr;
  QColor m_fillColour = QColor(30, 144, 255);
  QColor m_strokeColour = QColor(0, 0, 0);
  int m_strokeWidth = 1;
};
