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

  connect(newBtn, &QPushButton::clicked, this, &ControlPanel::RequestNew);
  connect(openBtn, &QPushButton::clicked, this, &ControlPanel::RequestOpen);
  connect(saveBtn, &QPushButton::clicked, this, &ControlPanel::RequestSave);
  connect(saveAsBtn, &QPushButton::clicked, this, &ControlPanel::RequestSaveAs);
  connect(closeBtn, &QPushButton::clicked, this, &ControlPanel::RequestClose);
  connect(undoBtn, &QPushButton::clicked, this, &ControlPanel::RequestUndo);
  connect(redoBtn, &QPushButton::clicked, this, &ControlPanel::RequestRedo);
  connect(clearBtn, &QPushButton::clicked, this, &ControlPanel::RequestClear);

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
