#include "shapepanel.h"

#include <QHBoxLayout>
#include <QPushButton>

#include "../svg/shape_registry.h"

ShapePanel::ShapePanel(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);
  for (const auto& trait : getRegistry()) {
    auto* btn = new QPushButton(QString::fromStdString(trait.uiLabel), this);
    connect(btn, &QPushButton::clicked, this, [this, factory = trait.create]() { emit request_shape(factory); });
    layout->addWidget(btn);
  }

  auto* clearBtn = new QPushButton("Clear", this);
  connect(clearBtn, &QPushButton::clicked, this, &ShapePanel::request_clear);
  layout->addWidget(clearBtn);
  layout->addStretch();
}
