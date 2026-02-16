#include "mainwidget.h"

#include <QVBoxLayout>

#include "../../canvas/core/mycanvas.h"
#include "../panels/colourpanel.h"
#include "../panels/controlpanel.h"
#include "../panels/shapepanel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Siddhant's Micro-Vector Editor");
  setMinimumSize(1000, 800);

  canvas_ = new MyCanvas(this);
  colour_panel_ = new ColourRibbon(this);
  control_panel_ = new ControlPanel(this);
  shape_panel_ = new ShapePanel(this);

  canvas_->setAttribute(Qt::WA_StyledBackground, true);
  canvas_->setStyleSheet("background-color: #c2c2c2;");
  colour_panel_->setAttribute(Qt::WA_StyledBackground, true);
  colour_panel_->setStyleSheet("background-color: #191919;");
  control_panel_->setAttribute(Qt::WA_StyledBackground, true);
  control_panel_->setStyleSheet("background-color: #101010;");
  shape_panel_->setAttribute(Qt::WA_StyledBackground, true);
  shape_panel_->setStyleSheet("background-color: #000000;");

  connect(shape_panel_, &ShapePanel::RequestShape, this, [this](auto factory) {
    auto shape = factory();
    shape->fillColour = colour_panel_->GetFillColour();
    shape->strokeColour = colour_panel_->GetStrokeColour();
    shape->strokeWidth = colour_panel_->GetStrokeWidth();
    canvas_->AddShape(std::move(shape));
  });

  connect(control_panel_, &ControlPanel::RequestClear, this, [this]() { canvas_->RemoveLastShape(); });
  connect(control_panel_, &ControlPanel::RequestUndo, this, [this]() { canvas_->Undo(); });
  connect(control_panel_, &ControlPanel::RequestRedo, this, [this]() { canvas_->Redo(); });
  connect(control_panel_, &ControlPanel::RequestNew, this, [this]() { NewFile(); });
  connect(control_panel_, &ControlPanel::RequestOpen, this, [this]() { OpenFile(); });
  connect(control_panel_, &ControlPanel::RequestSave, this, [this]() { Save(); });
  connect(control_panel_, &ControlPanel::RequestSaveAs, this, [this]() { SaveAs(); });
  connect(control_panel_, &ControlPanel::RequestClose, this, [this]() { CloseFile(); });

  auto applyColours = [this]() {
    canvas_->ApplyColourSpec(colour_panel_->GetFillColour(), colour_panel_->GetStrokeColour(), colour_panel_->GetStrokeWidth());
  };
  connect(colour_panel_, &ColourRibbon::colourChanged, this, applyColours);
  connect(colour_panel_, &ColourRibbon::strokeWidthChanged, this, applyColours);
  connect(colour_panel_, &ColourRibbon::fontChanged, this, [this](const QString& f) {
    canvas_->ApplyFont(f);
  });
  connect(colour_panel_, &ColourRibbon::fontSizeChanged, this, [this](int s) {
    canvas_->ApplyFontSize(s);
  });

  connect(shape_panel_, &ShapePanel::SketchModeToggled, this, [this](bool on) {
    canvas_->SetFreehandMode(on);
  });

  auto* full_screen = new QVBoxLayout(this);
  full_screen->setSpacing(0);
  full_screen->addWidget(control_panel_);
  full_screen->addWidget(shape_panel_);
  full_screen->addWidget(colour_panel_);
  full_screen->addWidget(canvas_, 1);
}
