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

  connect(shapePanel, &ShapePanel::request_shape, this, [this](auto factory) {
    auto shape = factory();
    shape->fillColour = colourPanel->getFillColour();
    shape->strokeColour = colourPanel->getStrokeColour();
    shape->strokeWidth = colourPanel->getStrokeWidth();
    mycanvas->addshape(std::move(shape));
  });

  connect(controlPanel, &ControlPanel::request_clear, this, [this]() { mycanvas->removelastshape(); });
  connect(controlPanel, &ControlPanel::request_undo, this, [this]() { mycanvas->undo(); });
  connect(controlPanel, &ControlPanel::request_redo, this, [this]() { mycanvas->redo(); });
  connect(controlPanel, &ControlPanel::request_new, this, [this]() { newFile(); });
  connect(controlPanel, &ControlPanel::request_open, this, [this]() { openFile(); });
  connect(controlPanel, &ControlPanel::request_save, this, [this]() { save(); });
  connect(controlPanel, &ControlPanel::request_saveas, this, [this]() { saveAs(); });
  connect(controlPanel, &ControlPanel::request_close, this, [this]() { closeFile(); });

  auto applyColours = [this]() {
    mycanvas->applyColourSpec(colourPanel->getFillColour(), colourPanel->getStrokeColour(), colourPanel->getStrokeWidth());
  };
  connect(colourPanel, &ColourRibbon::colourChanged, this, applyColours);
  connect(colourPanel, &ColourRibbon::strokeWidthChanged, this, applyColours);
  connect(colourPanel, &ColourRibbon::fontChanged, this, [this](const QString& f) {
    mycanvas->applyFont(f);
  });
  connect(colourPanel, &ColourRibbon::fontSizeChanged, this, [this](int s) {
    mycanvas->applyFontSize(s);
  });

  connect(shapePanel, &ShapePanel::sketchModeToggled, this, [this](bool on) {
    mycanvas->setFreehandMode(on);
  });

  auto* full_screen = new QVBoxLayout(this);
  full_screen->setSpacing(0);
  full_screen->addWidget(controlPanel);
  full_screen->addWidget(shapePanel);
  full_screen->addWidget(colourPanel);
  full_screen->addWidget(mycanvas, 1);
}

void MainWidget::newFile() {
  if (!mycanvas->getShapes().empty()) {
    QMessageBox box(this);
    box.setWindowTitle("New File");
    box.setText("Do you want to save before starting a new file?");
    box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Save);
    const int ret = box.exec();
    if (ret == QMessageBox::Cancel) return;
    if (ret == QMessageBox::Save) {
      if (currentFilePath.isEmpty()) {
        if (!saveAs()) return;
      } else {
        if (!saveToFile(currentFilePath)) return;
      }
    }
  }
  mycanvas->clearAll();
  currentFilePath.clear();
  setWindowTitle("Siddhant's Micro-Vector Editor");
}

void MainWidget::save() {
  if (currentFilePath.isEmpty()) {
    saveAs();
  } else {
    saveToFile(currentFilePath);
  }
}

void MainWidget::openFile() {
  QString path = QFileDialog::getOpenFileName(this, "Open SVG/XML File", QString(),
                                              "XML files (*.xml);;SVG files (*.svg);;All files (*)");
  if (path.isEmpty()) return;

  auto parsed = SvgParser::parseSvgFile(path.toStdString());
  auto loaded = Data2Vec::convertToShapes(parsed);
  if (loaded.empty()) {
    QMessageBox::warning(this, "Parse Error",
                         "Could not parse any shapes from:\n" + path);
    return;
  }

  mycanvas->clearAll();
  for (auto& s : loaded) mycanvas->addshape(std::move(s), false);
  currentFilePath = path;
  setWindowTitle("Siddhant's Micro-Vector Editor — " + QFileInfo(path).fileName());
}

bool MainWidget::saveToFile(const QString& path) {
  const bool ok = Vec2Svg::saveShapesToSvgFile(
      mycanvas->getShapes(), path.toStdString(),
      mycanvas->width(), mycanvas->height());
  if (!ok) {
    QMessageBox::warning(this, "Save Error", "Failed to save to:\n" + path);
    return false;
  }
  currentFilePath = path;
  setWindowTitle("Siddhant's Micro-Vector Editor — " + QFileInfo(path).fileName());
  return true;
}

bool MainWidget::saveAs() {
  QString path = QFileDialog::getSaveFileName(this, "Save As", currentFilePath,
                                              "XML files (*.xml);;SVG files (*.svg);;All files (*)");
  if (path.isEmpty()) return false;
  return saveToFile(path);
}

void MainWidget::closeFile() {
  if (mycanvas->getShapes().empty()) return;

  QMessageBox box(this);
  box.setWindowTitle("Close File");
  box.setText("Do you want to save before closing?");
  box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  box.setDefaultButton(QMessageBox::Save);
  const int ret = box.exec();

  if (ret == QMessageBox::Cancel) return;

  if (ret == QMessageBox::Save) {
    if (currentFilePath.isEmpty()) {
      if (!saveAs()) return;  // user cancelled save-as
    } else {
      if (!saveToFile(currentFilePath)) return;
    }
  }

  mycanvas->clearAll();
  currentFilePath.clear();
  setWindowTitle("Siddhant's Micro-Vector Editor");
}
