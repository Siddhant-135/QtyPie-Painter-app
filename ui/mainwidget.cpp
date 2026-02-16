#include "mainwidget.h"

#include <QVBoxLayout>

#include "../colourpanel.h"
#include "../svg/svg_convert.h"
#include "../svg/svg_parser.h"
#include "mycanvas.h"
#include "shapepanel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Siddhant's Micro-Vector Editor");
  setMinimumSize(1000, 800);

  mycanvas = new MyCanvas(this);
  colourPanel = new ColourRibbon(this);
  shapePanel = new ShapePanel(this);

  mycanvas->setAttribute(Qt::WA_StyledBackground, true);
  mycanvas->setStyleSheet("background-color: #c2c2c2;");
  colourPanel->setAttribute(Qt::WA_StyledBackground, true);
  colourPanel->setStyleSheet("background-color: #191919;");
  shapePanel->setAttribute(Qt::WA_StyledBackground, true);
  shapePanel->setStyleSheet("background-color: #000000;");

  auto parsed = parseSvgFile("svg/test_input.xml");
  auto preloadedShapes = convertToShapes(parsed);
  for (auto& s : preloadedShapes) mycanvas->addshape(std::move(s));

  connect(shapePanel, &ShapePanel::request_shape, this, [this](auto factory) {
    auto shape = factory();
    shape->fillColour = colourPanel->getFillColour();
    shape->strokeColour = colourPanel->getStrokeColour();
    shape->strokeWidth = colourPanel->getStrokeWidth();
    mycanvas->addshape(std::move(shape));
  });

  connect(shapePanel, &ShapePanel::request_clear, this, [this]() { mycanvas->removelastshape(); });

  auto applyColours = [this]() {
    mycanvas->applyColourSpec(colourPanel->getFillColour(), colourPanel->getStrokeColour(), colourPanel->getStrokeWidth());
  };
  connect(colourPanel, &ColourRibbon::colourChanged, this, applyColours);
  connect(colourPanel, &ColourRibbon::strokeWidthChanged, this, applyColours);

  auto* full_screen = new QVBoxLayout(this);
  full_screen->setSpacing(0);
  full_screen->addWidget(shapePanel);
  full_screen->addWidget(colourPanel);
  full_screen->addWidget(mycanvas, 1);
}
