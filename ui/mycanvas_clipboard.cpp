#include "mycanvas.h"

#include <algorithm>

#include "floatingmenu.h"
#include "../svg/Data2Shape.h"
#include "../svg/Shape2Data.h"

void MyCanvas::showFloatingMenu(const QPoint& globalPos) {
  if (!floatingMenu) {
    floatingMenu = new FloatingMenu(this);
    connect(floatingMenu, &FloatingMenu::copyClicked,   this, &MyCanvas::copySelected);
    connect(floatingMenu, &FloatingMenu::cutClicked,    this, &MyCanvas::cutSelected);
    connect(floatingMenu, &FloatingMenu::deleteClicked, this, &MyCanvas::deleteSelected);
    connect(floatingMenu, &FloatingMenu::pasteClicked,  this, &MyCanvas::pasteClipboard);
  }
  floatingMenu->move(globalPos);
  floatingMenu->show();
}

void MyCanvas::copySelected() {
  if (!selectedShape) return;
  clipboard = Shape2Data::convert(*selectedShape);
}

void MyCanvas::deleteSelected() {
  if (!selectedShape) return;
  auto it = std::find_if(shapes.begin(), shapes.end(),
    [this](const auto& sp) { return sp.get() == selectedShape; });
  if (it != shapes.end()) {
    size_t idx = static_cast<size_t>(it - shapes.begin());
    SvgTag data = Shape2Data::convert(**it);
    undoRedo.recordRemove(idx, data);
    shapes.erase(it);
  }
  selectedShape = nullptr;
  update();
}

void MyCanvas::cutSelected() {
  copySelected();
  deleteSelected();
}

void MyCanvas::pasteClipboard() {
  if (!clipboard) return;
  auto shape = Data2Shape::convert(*clipboard);
  if (!shape) return;
  shape->move_obj(20, 20);  // offset so paste isn't on top of original
  SvgTag data = Shape2Data::convert(*shape);
  shapes.push_back(std::move(shape));
  undoRedo.recordAdd(shapes.size() - 1, data);
  update();
}
