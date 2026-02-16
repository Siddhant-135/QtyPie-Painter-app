#pragma once

#include <QColor>
#include <QString>
#include <QWidget>

class QComboBox;
class QPushButton;
class QSpinBox;

class ColourRibbon : public QWidget {
  Q_OBJECT
 public:
  explicit ColourRibbon(QWidget* parent = nullptr);

  void PickColour(QColor& target, const QString& header);
  void SetStrokeWidth(int width);

  const QColor& GetFillColour() const;
  const QColor& GetStrokeColour() const;
  int GetStrokeWidth() const;
  const QString& GetFontFamily() const;
  int GetFontSize() const;

 signals:
  void colourChanged(const QColor& colour);
  void strokeWidthChanged(int width);
  void fontChanged(const QString& family);
  void fontSizeChanged(int size);

 private:
  QPushButton* fill_btn_ = nullptr;
  QPushButton* stroke_btn_ = nullptr;
  QSpinBox* width_spin_ = nullptr;
  QComboBox* font_combo_ = nullptr;
  QSpinBox* font_size_spin_ = nullptr;
  QColor fill_colour_ = QColor(30, 144, 255);
  QColor stroke_colour_ = QColor(0, 0, 0);
  int stroke_width_ = 1;
  QString font_family_ = "Arial";
  int font_size_ = 15;
};
