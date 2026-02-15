#include "colourpanel.h"
#include <QColorDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

ColourRibbon::ColourRibbon(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);

  fillcBtn = new QPushButton("Fill Colour", this); // simple button that opens the internal QColorDialog, returning hex value to the relevant slot.
  connect(fillcBtn, &QPushButton::clicked, this, [this]() {
    pickColour(m_fillColour, "Choose Fill Colour");
  });
  layout->addWidget(fillcBtn);
  layout->addSpacing(12);

  strokecBtn = new QPushButton("Stroke Colour", this); // same for stroke colour
  connect(strokecBtn, &QPushButton::clicked, this, [this]() {
    pickColour(m_strokeColour, "Choose Stroke Colour");
  });
  layout->addWidget(strokecBtn);
  layout->addSpacing(12);

  layout->addWidget(new QLabel("Stroke Width:", this)); // SpinBox is a counter for the stroke width. 0 = no stroke, rest pixelwise
  width_counter_btn = new QSpinBox(this);
  width_counter_btn->setRange(0, 20); // <- TO BE BE CHANGED if you need very thick borders, this is way more than enought btw.
  width_counter_btn->setValue(m_strokeWidth);
  connect(width_counter_btn, &QSpinBox::valueChanged, this, [this](int v) {
    setStrokeWidth(v);
  });
  layout->addWidget(width_counter_btn);
  layout->addStretch(); // UI thing, stretches to take up space
}

void ColourRibbon::pickColour(QColor& target, const QString& header) {
  QColor c = QColorDialog::getColor(target, this, header);
  if (c.isValid()) {
    target = c;
    emit colourChanged(c); // This is connected to specific slots in the canvas. Kept only one for simplicity. 
  }
}
