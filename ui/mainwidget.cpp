#include "mainwidget.h"

#include <QDebug>
#include <QVBoxLayout>

#include "../svg/Data2Vec.h"
#include "../svg/Svg2Data.h"
#include "../svg/Vec2Svg.h"
#include "colourpanel.h"
#include "controlpanel.h"
#include "mycanvas.h"
#include "shapepanel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Siddhant's Micro-Vector Editor");
  setMinimumSize(1000, 800);

  mycanvas = new MyCanvas(this);
  colourPanel = new ColourRibbon(this);
  controlPanel = new ControlPanel(this);
  shapePanel = new ShapePanel(this);

  mycanvas->setAttribute(Qt::WA_StyledBackground, true);
  mycanvas->setStyleSheet("background-color: #c2c2c2;");
  colourPanel->setAttribute(Qt::WA_StyledBackground, true);
  colourPanel->setStyleSheet("background-color: #191919;");
  controlPanel->setAttribute(Qt::WA_StyledBackground, true);
  controlPanel->setStyleSheet("background-color: #101010;");
  shapePanel->setAttribute(Qt::WA_StyledBackground, true);
  shapePanel->setStyleSheet("background-color: #000000;");

  auto parsed = SvgParser::parseSvgFile("svg/test_input.xml");
  auto preloadedShapes = Data2Vec::convertToShapes(parsed);
  for (auto& s : preloadedShapes) mycanvas->addshape(std::move(s), false);

  connect(shapePanel, &ShapePanel::request_shape, this, [this](auto factory) {
    auto shape = factory();
    shape->fillColour = colourPanel->getFillColour();
    shape->strokeColour = colourPanel->getStrokeColour();
    shape->strokeWidth = colourPanel->getStrokeWidth();
    mycanvas->addshape(std::move(shape));
  });

  connect(controlPanel, &ControlPanel::request_clear, this, [this]() { mycanvas->removelastshape(); });
  connect(controlPanel, &ControlPanel::request_undo, this, [this]() {
    mycanvas->undo();
  });
  connect(controlPanel, &ControlPanel::request_redo, this, [this]() {
    mycanvas->redo();
  });
  connect(controlPanel, &ControlPanel::request_save, this, [this]() {
    const std::string outPath = "output.xml";
    const bool ok = Vec2Svg::saveShapesToSvgFile(mycanvas->getShapes(), outPath, mycanvas->width(), mycanvas->height());
    if (!ok) {
      qWarning() << "Save failed:" << QString::fromStdString(outPath);
      return;
    }
  });

  auto applyColours = [this]() {
    mycanvas->applyColourSpec(colourPanel->getFillColour(), colourPanel->getStrokeColour(), colourPanel->getStrokeWidth());
  };
  connect(colourPanel, &ColourRibbon::colourChanged, this, applyColours);
  connect(colourPanel, &ColourRibbon::strokeWidthChanged, this, applyColours);

  auto* full_screen = new QVBoxLayout(this);
  full_screen->setSpacing(0);
  full_screen->addWidget(controlPanel);
  full_screen->addWidget(shapePanel);
  full_screen->addWidget(colourPanel);
  full_screen->addWidget(mycanvas, 1);
}
