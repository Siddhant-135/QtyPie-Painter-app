#ifndef UI_PANELS_COLOURPANEL_H_
#define UI_PANELS_COLOURPANEL_H_

#include <QColor>
#include <QString>
#include <QWidget>

#include "../../config/config.h"

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
  QColor fill_colour_ = config::kDefaultFillColour;
  QColor stroke_colour_ = config::kDefaultStrokeColour;
  int stroke_width_ = config::kDefaultStrokeWidth;
  QString font_family_ = config::kDefaultFontFamily;
  int font_size_ = config::kDefaultFontSize;
};

#endif  // UI_PANELS_COLOURPANEL_H_
