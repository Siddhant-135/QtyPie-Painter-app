#pragma once

#include <QPointF>
#include <QWidget>

#include <memory>
#include <optional>
#include <vector>

#include "../shapes/shapes.h"
#include "../svg/parser/Svg2Data.h"
#include "../undo/undoredo.h"

class QMouseEvent;
class QPaintEvent;
class FloatingMenu;

class MyCanvas : public QWidget {
  Q_OBJECT
 public:
  explicit MyCanvas(QWidget* parent = nullptr);
  void AddShape(std::unique_ptr<Shape> s, bool recordUndo = true);
  void RemoveLastShape();
  void ApplyColourSpec(QColor fill, QColor stroke, int width);
  void ApplyFont(const QString& family);
  void ApplyFontSize(int size);
  void EditSelectedText();
  const std::vector<std::unique_ptr<Shape>>& GetShapes() const;
  void Undo();
  void Redo();
  void ClearAll();
  void SetFreehandMode(bool on);

 protected:
  void paintEvent(QPaintEvent* event) override;
  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;

 private:
  void ShowFloatingMenu(const QPoint& globalPos);
  void CopySelected();
  void DeleteSelected();
  void CutSelected();
  void PasteClipboard();

  std::vector<std::unique_ptr<Shape>> shapes_;
  Shape* selected_shape_ = nullptr;
  bool dragging_ = false;
  int active_handle_ = -1;
  QPointF last_mouse_pos_;

  FloatingMenu* floating_menu_ = nullptr;
  std::optional<SvgTag> clipboard_;

  UndoRedoManager undo_redo_;
  std::optional<SvgTag> pre_drag_snapshot_;
  size_t pre_drag_index_ = 0;

  bool freehand_mode_ = false;
  bool freehand_drawing_ = false;
  std::vector<QPointF> freehand_pts_;
};
