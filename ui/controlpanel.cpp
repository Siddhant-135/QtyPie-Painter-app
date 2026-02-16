#include "controlpanel.h"

#include <QHBoxLayout>
#include <QPushButton>

ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent) {
  auto* layout = new QHBoxLayout(this);

  auto* newBtn = new QPushButton("New", this);
  auto* openBtn = new QPushButton("Open", this);
  auto* saveBtn = new QPushButton("Save", this);
  auto* saveAsBtn = new QPushButton("Save As", this);
  auto* closeBtn = new QPushButton("Close", this);
  auto* undoBtn = new QPushButton("Undo", this);
  auto* redoBtn = new QPushButton("Redo", this);
  auto* clearBtn = new QPushButton("Clear", this);

  connect(newBtn, &QPushButton::clicked, this, &ControlPanel::request_new);
  connect(openBtn, &QPushButton::clicked, this, &ControlPanel::request_open);
  connect(saveBtn, &QPushButton::clicked, this, &ControlPanel::request_save);
  connect(saveAsBtn, &QPushButton::clicked, this, &ControlPanel::request_saveas);
  connect(closeBtn, &QPushButton::clicked, this, &ControlPanel::request_close);
  connect(undoBtn, &QPushButton::clicked, this, &ControlPanel::request_undo);
  connect(redoBtn, &QPushButton::clicked, this, &ControlPanel::request_redo);
  connect(clearBtn, &QPushButton::clicked, this, &ControlPanel::request_clear);

  layout->addWidget(newBtn);
  layout->addWidget(openBtn);
  layout->addWidget(saveBtn);
  layout->addWidget(saveAsBtn);
  layout->addWidget(closeBtn);
  layout->addWidget(undoBtn);
  layout->addWidget(redoBtn);
  layout->addWidget(clearBtn);
  layout->addStretch();
}
