#include "mainwidget.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QVBoxLayout>

#include "../svg/Data2Vec.h"
#include "../svg/Svg2Data.h"
#include "../svg/Vec2Svg.h"
#include "../canvas/mycanvas.h"
#include "colourpanel.h"
#include "controlpanel.h"
#include "shapepanel.h"

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

void MainWidget::NewFile() {
  if (!canvas_->GetShapes().empty()) {
    QMessageBox box(this);
    box.setWindowTitle("New File");
    box.setText("Do you want to save before starting a new file?");
    box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Save);
    const int ret = box.exec();
    if (ret == QMessageBox::Cancel) return;
    if (ret == QMessageBox::Save) {
      if (current_file_path_.isEmpty()) {
        if (!SaveAs()) return;
      } else {
        if (!SaveToFile(current_file_path_)) return;
      }
    }
  }
  canvas_->ClearAll();
  current_file_path_.clear();
  setWindowTitle("Siddhant's Micro-Vector Editor");
}

void MainWidget::Save() {
  if (current_file_path_.isEmpty()) {
    SaveAs();
  } else {
    SaveToFile(current_file_path_);
  }
}

void MainWidget::OpenFile() {
  QString path = QFileDialog::getOpenFileName(this, "Open SVG/XML File", QString(),
                                              "XML files (*.xml);;SVG files (*.svg);;All files (*)");
  if (path.isEmpty()) return;

  auto parsed = SvgParser::ParseSvgFile(path.toStdString());
  auto loaded = Data2Vec::ConvertToShapes(parsed);
  if (loaded.empty()) {
    QMessageBox::warning(this, "Parse Error",
                         "Could not parse any shapes from:\n" + path);
    return;
  }

  canvas_->ClearAll();
  for (auto& s : loaded) canvas_->AddShape(std::move(s), false);
  current_file_path_ = path;
  setWindowTitle("Siddhant's Micro-Vector Editor — " + QFileInfo(path).fileName());
}

bool MainWidget::SaveToFile(const QString& path) {
  const bool ok = Vec2Svg::SaveShapesToSvgFile(
      canvas_->GetShapes(), path.toStdString(),
      canvas_->width(), canvas_->height());
  if (!ok) {
    QMessageBox::warning(this, "Save Error", "Failed to save to:\n" + path);
    return false;
  }
  current_file_path_ = path;
  setWindowTitle("Siddhant's Micro-Vector Editor — " + QFileInfo(path).fileName());
  return true;
}

bool MainWidget::SaveAs() {
  QString path = QFileDialog::getSaveFileName(this, "Save As", current_file_path_,
                                              "XML files (*.xml);;SVG files (*.svg);;All files (*)");
  if (path.isEmpty()) return false;
  return SaveToFile(path);
}

void MainWidget::CloseFile() {
  if (canvas_->GetShapes().empty()) return;

  QMessageBox box(this);
  box.setWindowTitle("Close File");
  box.setText("Do you want to save before closing?");
  box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  box.setDefaultButton(QMessageBox::Save);
  const int ret = box.exec();

  if (ret == QMessageBox::Cancel) return;

  if (ret == QMessageBox::Save) {
    if (current_file_path_.isEmpty()) {
      if (!SaveAs()) return;  // user cancelled save-as
    } else {
      if (!SaveToFile(current_file_path_)) return;
    }
  }

  canvas_->ClearAll();
  current_file_path_.clear();
  setWindowTitle("Siddhant's Micro-Vector Editor");
}
