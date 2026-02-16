#include "shapepanel.h"
#include <QHBoxLayout>
#include <QPushButton>
#include "../svg/shape_registry.h"

ShapePanel::ShapePanel(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);
  for (const auto& trait : ShapeRegistry::getRegistry()) {
    auto* btn = new QPushButton(QString::fromStdString(trait.uiLabel), this);
    connect(btn, &QPushButton::clicked, this, [this, factory = trait.create]() { emit request_shape(factory); });
    layout->addWidget(btn);
  }
  layout->addStretch();
}
