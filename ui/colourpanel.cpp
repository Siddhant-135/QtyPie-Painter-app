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

  fillcBtn = new QPushButton("Fill Colour", this);
  connect(fillcBtn, &QPushButton::clicked, this, [this]() { pickColour(m_fillColour, "Choose Fill Colour"); });
  layout->addWidget(fillcBtn);
  layout->addSpacing(12);

  strokecBtn = new QPushButton("Stroke Colour", this);
  connect(strokecBtn, &QPushButton::clicked, this, [this]() { pickColour(m_strokeColour, "Choose Stroke Colour"); });
  layout->addWidget(strokecBtn);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Stroke Width:", this));
  width_counter_btn = new QSpinBox(this);
  width_counter_btn->setRange(1, 20);
  width_counter_btn->setValue(m_strokeWidth);
  connect(width_counter_btn, &QSpinBox::valueChanged, this, [this](int v) { setStrokeWidth(v); });
  layout->addWidget(width_counter_btn);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Text Font:", this));
  fontCombo = new QComboBox(this);
  fontCombo->addItems(QFontDatabase::families());
  fontCombo->setCurrentText(m_fontFamily);
  connect(fontCombo, &QComboBox::currentTextChanged, this, [this](const QString& f) {
    m_fontFamily = f;
    emit fontChanged(f);
  });
  layout->addWidget(fontCombo);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Font Size:", this));
  fontSizeSpin = new QSpinBox(this);
  fontSizeSpin->setRange(10, 40);
  fontSizeSpin->setValue(m_fontSize);
  connect(fontSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int v) {
    m_fontSize = v;
    emit fontSizeChanged(v);
  });
  layout->addWidget(fontSizeSpin);
  layout->addStretch();
}

void ColourRibbon::pickColour(QColor& target, const QString& header) {
  QColor c = QColorDialog::getColor(target, this, header);
  if (!c.isValid()) return;
  target = c;
  emit colourChanged(c);
}

void ColourRibbon::setStrokeWidth(int width) {
  m_strokeWidth = width;
  emit strokeWidthChanged(width);
}

const QColor& ColourRibbon::getFillColour() const { return m_fillColour; }
const QColor& ColourRibbon::getStrokeColour() const { return m_strokeColour; }
int ColourRibbon::getStrokeWidth() const { return m_strokeWidth; }
const QString& ColourRibbon::getFontFamily() const { return m_fontFamily; }
int ColourRibbon::getFontSize() const { return m_fontSize; }
