#include <QInputDialog>
#include <QLineEdit>

#include "../../shapes/rectangular/rectangle.h"
#include "../../svg/export/Shape2Data.h"
#include "mycanvas.h"

MyCanvas::MyCanvas(QWidget* parent) : QWidget(parent) {}

void MyCanvas::AddShape(std::unique_ptr<Shape> s, bool recordUndo) {
  shapes_.push_back(std::move(s));
  if (recordUndo) {
    SvgTag data = Shape2Data::Convert(*shapes_.back());
    undo_redo_.RecordAdd(shapes_.size() - 1, data);
  }
  update();
}

void MyCanvas::RemoveLastShape() {
  if (shapes_.empty()) return;
  SvgTag data = Shape2Data::Convert(*shapes_.back());
  undo_redo_.RecordRemove(shapes_.size() - 1, data);
  shapes_.pop_back();
  update();
}

void MyCanvas::ApplyColourSpec(QColor fill, QColor stroke, int width) {
  if (!selected_shape_) return;
  for (size_t i = 0; i < shapes_.size(); ++i) {
    if (shapes_[i].get() != selected_shape_) continue;
    SvgTag old = Shape2Data::Convert(*selected_shape_);
    selected_shape_->fillColour = fill;
    selected_shape_->strokeColour = stroke;
    selected_shape_->strokeWidth = width;
    undo_redo_.RecordModify(i, old);
    break;
  }
  update();
}

void MyCanvas::ApplyFont(const QString& family) {
  if (!selected_shape_) return;
  auto* tb = dynamic_cast<TextBox*>(selected_shape_);
  if (!tb) return;
  for (size_t i = 0; i < shapes_.size(); ++i) {
    if (shapes_[i].get() != selected_shape_) continue;
    SvgTag old = Shape2Data::Convert(*selected_shape_);
    tb->fontFamily = family;
    undo_redo_.RecordModify(i, old);
    break;
  }
  update();
}

void MyCanvas::ApplyFontSize(int size) {
  if (!selected_shape_) return;
  auto* tb = dynamic_cast<TextBox*>(selected_shape_);
  if (!tb) return;
  for (size_t i = 0; i < shapes_.size(); ++i) {
    if (shapes_[i].get() != selected_shape_) continue;
    SvgTag old = Shape2Data::Convert(*selected_shape_);
    tb->fontSize = size;
    undo_redo_.RecordModify(i, old);
    break;
  }
  update();
}

void MyCanvas::EditSelectedText() {
  if (!selected_shape_) return;
  auto* tb = dynamic_cast<TextBox*>(selected_shape_);
  if (!tb) return;
  bool ok = false;
  QString result = QInputDialog::getText(
      this, "Edit Text", "Enter text:", QLineEdit::Normal, tb->text_line, &ok);
  if (ok && !result.isEmpty()) {
    for (size_t i = 0; i < shapes_.size(); ++i) {
      if (shapes_[i].get() != selected_shape_) continue;
      SvgTag old = Shape2Data::Convert(*selected_shape_);
      tb->text_line = result;
      undo_redo_.RecordModify(i, old);
      break;
    }
    update();
  }
}

const std::vector<std::unique_ptr<Shape>>& MyCanvas::GetShapes() const {
  return shapes_;
}

void MyCanvas::SetFreehandMode(bool on) {
  freehand_mode_ = on;
  freehand_drawing_ = false;
  freehand_pts_.clear();
}

void MyCanvas::Undo() {
  selected_shape_ = nullptr;
  undo_redo_.Undo(shapes_);
  update();
}

void MyCanvas::Redo() {
  selected_shape_ = nullptr;
  undo_redo_.Redo(shapes_);
  update();
}

void MyCanvas::ClearAll() {
  selected_shape_ = nullptr;
  shapes_.clear();
  undo_redo_ = UndoRedoManager();
  freehand_drawing_ = false;
  freehand_pts_.clear();
  update();
}
