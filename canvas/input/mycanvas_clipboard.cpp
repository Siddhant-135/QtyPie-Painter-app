#include <algorithm>

#include "../../config/config.h"
#include "../../svg/export/Shape2Data.h"
#include "../../svg/import/Data2Shape.h"
#include "../../ui/menus/floatingmenu.h"
#include "../core/mycanvas.h"

void MyCanvas::ShowFloatingMenu(const QPoint& globalPos) {
  if (!floating_menu_) {
    floating_menu_ = new FloatingMenu(this);
    connect(floating_menu_, &FloatingMenu::CopyClicked, this,
            &MyCanvas::CopySelected);
    connect(floating_menu_, &FloatingMenu::CutClicked, this,
            &MyCanvas::CutSelected);
    connect(floating_menu_, &FloatingMenu::DeleteClicked, this,
            &MyCanvas::DeleteSelected);
    connect(floating_menu_, &FloatingMenu::PasteClicked, this,
            &MyCanvas::PasteClipboard);
  }
  floating_menu_->move(globalPos);
  floating_menu_->show();
}

void MyCanvas::CopySelected() {
  if (!selected_shape_) return;
  clipboard_ = Shape2Data::Convert(*selected_shape_);
}

void MyCanvas::DeleteSelected() {
  if (!selected_shape_) return;
  auto it = std::find_if(
      shapes_.begin(), shapes_.end(),
      [this](const auto& sp) { return sp.get() == selected_shape_; });
  if (it != shapes_.end()) {
    size_t idx = static_cast<size_t>(it - shapes_.begin());
    SvgTag data = Shape2Data::Convert(**it);
    undo_redo_.RecordRemove(idx, data);
    shapes_.erase(it);
  }
  selected_shape_ = nullptr;
  update();
}

void MyCanvas::CutSelected() {
  CopySelected();
  DeleteSelected();
}

void MyCanvas::PasteClipboard() {
  if (!clipboard_) return;
  auto shape = Data2Shape::Convert(*clipboard_);
  if (!shape) return;
  shape->MoveObj(
      config::kPasteOffsetX,
      config::kPasteOffsetY);  // offset so paste isn't on top of original
  SvgTag data = Shape2Data::Convert(*shape);
  shapes_.push_back(std::move(shape));
  undo_redo_.RecordAdd(shapes_.size() - 1, data);
  update();
}
