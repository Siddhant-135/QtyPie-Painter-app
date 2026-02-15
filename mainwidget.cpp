#include "mainwidget.h"

#include <functional>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "mycanvas.h"
#include "shapes.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Siddhant's Micro-Vector Editor");
  setMinimumSize(1000, 800); // Painful to keep resizing otherwise
  mycanvas = new MyCanvas(this);

  const std::vector<std::pair<QString, std::function<std::unique_ptr<Shape>()>>> shapeTypes = // Makes it way easier to add new shapes
  { 
    {"Rectangle", [] { return std::make_unique<Rectangle>(); }}, {"Square", [] { return std::make_unique<Square>(); }}, 
    {"Circle",    [] { return std::make_unique<Circle>(); }}, {"Line", [] { return std::make_unique<Line>(); }},
  };

  auto* button_panel = new QHBoxLayout(); // Horizontal layout for the buttons

  for (const auto& [name, factory] : shapeTypes) {
      auto* btn = new QPushButton(name, this);
      const auto f = factory; 
      connect(btn, &QPushButton::clicked, this, [this, f]() {mycanvas->addshape(f());} ); // Relates clicking the button with addition function
      button_panel->addWidget(btn);
  }

  auto* clearBtn = new QPushButton("Clear", this);
  connect(clearBtn, &QPushButton::clicked, this, [this]() {
      mycanvas->removelastshape(); });
  button_panel->addWidget(clearBtn);
  button_panel->addStretch();

  auto* full_screen = new QVBoxLayout(this);
  full_screen->addLayout(button_panel);
  full_screen->addWidget(mycanvas, 1);
}
