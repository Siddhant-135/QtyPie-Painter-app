#include "controlpanel.h"

#include <QHBoxLayout>
#include <QPushButton>

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);

  auto* undoBtn = new QPushButton("Undo", this);
  auto* redoBtn = new QPushButton("Redo", this);
  auto* saveBtn = new QPushButton("Save", this);
  auto* clearBtn = new QPushButton("Clear", this);

  connect(undoBtn, &QPushButton::clicked, this, &ControlPanel::request_undo);
  connect(redoBtn, &QPushButton::clicked, this, &ControlPanel::request_redo);
  connect(saveBtn, &QPushButton::clicked, this, &ControlPanel::request_save);
  connect(clearBtn, &QPushButton::clicked, this, &ControlPanel::request_clear);

  layout->addWidget(undoBtn);
  layout->addWidget(redoBtn);
  layout->addWidget(saveBtn);
  layout->addWidget(clearBtn);
  layout->addStretch();
}
