#pragma once

#include <QPointF>
#include <QWidget>

#include <memory>
#include <optional>
#include <vector>

#include "../shapes/shapes.h"
#include "../svg/Svg2Data.h"
#include "undoredo.h"

class QMouseEvent;
class QPaintEvent;
class FloatingMenu;

class MyCanvas : public QWidget {
  Q_OBJECT
 public:
  explicit MyCanvas(QWidget* parent = nullptr);
  void addshape(std::unique_ptr<Shape> s, bool recordUndo = true);
  void removelastshape();
  void applyColourSpec(QColor fill, QColor stroke, int width);
  void applyFont(const QString& family);
  void applyFontSize(int size);
  void editSelectedText();
  const std::vector<std::unique_ptr<Shape>>& getShapes() const;
  void undo();
  void redo();
  void clearAll();
  void setFreehandMode(bool on);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  void showFloatingMenu(const QPoint& globalPos);
  void copySelected();
  void deleteSelected();
  void cutSelected();
  void pasteClipboard();

  std::vector<std::unique_ptr<Shape>> shapes;
  Shape* selectedShape = nullptr;
  bool dragging = false;
  int activeHandle = -1;
  QPointF lastMousePos;

  FloatingMenu* floatingMenu = nullptr;
  std::optional<SvgTag> clipboard;

  UndoRedoManager undoRedo;
  std::optional<SvgTag> preDragSnapshot;
  size_t preDragIndex = 0;

  bool freehandMode = false;
  bool freehandDrawing = false;
  std::vector<QPointF> freehandPts;
};
