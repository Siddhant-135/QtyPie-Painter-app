#include "shapepanel.h"
#include <QHBoxLayout>
#include <QPushButton>
#include "../svg/shape_registry.h"

ShapePanel::ShapePanel(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);
  for (const auto& trait : ShapeRegistry::getRegistry()) {
    if (trait.uiLabel.empty()) continue;
    auto* btn = new QPushButton(QString::fromStdString(trait.uiLabel), this);
    connect(btn, &QPushButton::clicked, this, [this, factory = trait.create]() {
      emit sketchModeToggled(false);
      emit request_shape(factory);
    });
    layout->addWidget(btn);
  }
  auto* sketchBtn = new QPushButton("Sketch", this);
  sketchBtn->setCheckable(true);
  connect(sketchBtn, &QPushButton::toggled, this, &ShapePanel::sketchModeToggled);
  layout->addWidget(sketchBtn);
  layout->addStretch();
}
