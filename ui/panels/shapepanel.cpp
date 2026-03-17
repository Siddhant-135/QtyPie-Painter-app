#include "shapepanel.h"

#include <QHBoxLayout>
#include <QPushButton>

#include "../../config/config.h"
#include "../../svg/registry/shape_registry.h"

ShapePanel::ShapePanel(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);
  for (const auto& trait : ShapeRegistry::GetRegistry()) {
    if (trait.uiLabel.empty()) continue;
    auto* btn = new QPushButton(QString::fromStdString(trait.uiLabel), this);
    connect(btn, &QPushButton::clicked, this, [this, factory = trait.create]() {
      emit SketchModeToggled(false);
      emit RequestShape(factory);
    });
    layout->addWidget(btn);
  }
  auto* sketchBtn = new QPushButton("Sketch", this);
  sketchBtn->setCheckable(true);
  sketchBtn->setStyleSheet("QPushButton:checked { background-color: " +
                           config::kSketchCheckedColour + "; color: white; }");
  connect(sketchBtn, &QPushButton::toggled, this,
          &ShapePanel::SketchModeToggled);
  layout->addWidget(sketchBtn);
  layout->addStretch();
}
