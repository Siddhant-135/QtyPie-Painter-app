#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "../../canvas/core/mycanvas.h"
#include "../../svg/export/Vec2Svg.h"
#include "../../svg/import/Data2Vec.h"
#include "../../svg/parser/Svg2Data.h"
#include "mainwidget.h"

void MainWidget::NewFile() {
  if (!canvas_->GetShapes().empty()) {
    QMessageBox box(this);
    box.setWindowTitle("New File");
    box.setText("Do you want to save before starting a new file?");
    box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                           QMessageBox::Cancel);
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
  QString path = QFileDialog::getOpenFileName(
      this, "Open SVG/XML File", QString(),
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
  setWindowTitle("Siddhant's Micro-Vector Editor — " +
                 QFileInfo(path).fileName());
}

bool MainWidget::SaveToFile(const QString& path) {
  const bool ok =
      Vec2Svg::SaveShapesToSvgFile(canvas_->GetShapes(), path.toStdString(),
                                   canvas_->width(), canvas_->height());
  if (!ok) {
    QMessageBox::warning(this, "Save Error", "Failed to save to:\n" + path);
    return false;
  }
  current_file_path_ = path;
  setWindowTitle("Siddhant's Micro-Vector Editor — " +
                 QFileInfo(path).fileName());
  return true;
}

bool MainWidget::SaveAs() {
  QString path = QFileDialog::getSaveFileName(
      this, "Save As", current_file_path_,
      "XML files (*.xml);;SVG files (*.svg);;All files (*)");
  if (path.isEmpty()) return false;
  return SaveToFile(path);
}

void MainWidget::CloseFile() {
  if (canvas_->GetShapes().empty()) return;

  QMessageBox box(this);
  box.setWindowTitle("Close File");
  box.setText("Do you want to save before closing?");
  box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                         QMessageBox::Cancel);
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

  canvas_->ClearAll();
  current_file_path_.clear();
  setWindowTitle("Siddhant's Micro-Vector Editor");
}
