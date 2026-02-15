#include "mainwidget.h"

#include <functional>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "colourpanel.h"
#include "shapepanel.h"
#include "mycanvas.h"
#include "shapes.h"
#include "svg_parser.h"
#include "svg_convert.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Siddhant's Micro-Vector Editor");
    setMinimumSize(1000, 800);

    mycanvas    = new MyCanvas(this);
    colourPanel = new ColourRibbon(this);
    auto* shapePanel = new ShapePanel(this); 

    auto parsed = parseSvgFile("test_input.xml");
    auto preloadedShapes = toShapes(parsed);
    for (auto& s : preloadedShapes)
    mycanvas->addshape(std::move(s));

    // Connections of the Shape Panel (and Clear)
    connect(shapePanel, &ShapePanel::request_shape, this, [this](auto factory) {
        auto shape = factory();
        shape->fillColour   = colourPanel->getFillColour();
        shape->strokeColour = colourPanel->getStrokeColour();
        shape->strokeWidth  = colourPanel->getStrokeWidth();
        mycanvas->addshape(std::move(shape));
    });

    connect(shapePanel, &ShapePanel::request_clear, this, [this]() {
        mycanvas->removelastshape(); 
    });

    // Connections of the Colour Panel
    auto applyColours = [this]() {
    mycanvas->applyColourSpec(colourPanel->getFillColour(), colourPanel->getStrokeColour(), colourPanel->getStrokeWidth());
    };
    connect(colourPanel, &ColourRibbon::colourChanged, this, applyColours);
    connect(colourPanel, &ColourRibbon::strokeWidthChanged, this, applyColours);

    // FULL LAYOUT (QVBOX)
    auto* full_screen = new QVBoxLayout(this);
    // full_screen->setContentsMargins(0, 0, 0, 0); 
    full_screen->setSpacing(0);
    full_screen->addWidget(shapePanel);   
    full_screen->addWidget(colourPanel);  
    full_screen->addWidget(mycanvas, 1); // the 1 lets it take all remaining space
}