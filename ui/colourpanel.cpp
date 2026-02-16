#include "colourpanel.h"

#include <QColorDialog>
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
  width_counter_btn->setRange(0, 20);
  width_counter_btn->setValue(m_strokeWidth);
  connect(width_counter_btn, &QSpinBox::valueChanged, this, [this](int v) { setStrokeWidth(v); });
  layout->addWidget(width_counter_btn);
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
