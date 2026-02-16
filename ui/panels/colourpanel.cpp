#include "colourpanel.h"

#include <QColorDialog>
#include <QComboBox>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

ColourRibbon::ColourRibbon(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);

  fill_btn_ = new QPushButton("Fill Colour", this);
  connect(fill_btn_, &QPushButton::clicked, this, [this]() { PickColour(fill_colour_, "Choose Fill Colour"); });
  layout->addWidget(fill_btn_);
  layout->addSpacing(12);

  stroke_btn_ = new QPushButton("Stroke Colour", this);
  connect(stroke_btn_, &QPushButton::clicked, this, [this]() { PickColour(stroke_colour_, "Choose Stroke Colour"); });
  layout->addWidget(stroke_btn_);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Stroke Width:", this));
  width_spin_ = new QSpinBox(this);
  width_spin_->setRange(1, 20);
  width_spin_->setValue(stroke_width_);
  connect(width_spin_, &QSpinBox::valueChanged, this, [this](int v) { SetStrokeWidth(v); });
  layout->addWidget(width_spin_);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Text Font:", this));
  font_combo_ = new QComboBox(this);
  font_combo_->addItems(QFontDatabase::families());
  font_combo_->setCurrentText(font_family_);
  connect(font_combo_, &QComboBox::currentTextChanged, this, [this](const QString& f) {
    font_family_ = f;
    emit fontChanged(f);
  });
  layout->addWidget(font_combo_);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Font Size:", this));
  font_size_spin_ = new QSpinBox(this);
  font_size_spin_->setRange(10, 40);
  font_size_spin_->setValue(font_size_);
  connect(font_size_spin_, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int v) {
    font_size_ = v;
    emit fontSizeChanged(v);
  });
  layout->addWidget(font_size_spin_);
  layout->addStretch();
}

void ColourRibbon::PickColour(QColor& target, const QString& header) {
  QColor c = QColorDialog::getColor(target, this, header);
  if (!c.isValid()) return;
  target = c;
  emit colourChanged(c);
}

void ColourRibbon::SetStrokeWidth(int width) {
  stroke_width_ = width;
  emit strokeWidthChanged(width);
}

const QColor& ColourRibbon::GetFillColour() const { return fill_colour_; }
const QColor& ColourRibbon::GetStrokeColour() const { return stroke_colour_; }
int ColourRibbon::GetStrokeWidth() const { return stroke_width_; }
const QString& ColourRibbon::GetFontFamily() const { return font_family_; }
int ColourRibbon::GetFontSize() const { return font_size_; }
