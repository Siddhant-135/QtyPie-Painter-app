#ifndef CANVAS_UNDO_UNDOREDO_H_
#define CANVAS_UNDO_UNDOREDO_H_

#include <cstddef>
#include <memory>
#include <stack>
#include <vector>

#include "../../svg/parser/Svg2Data.h"

struct Shape;
enum class ChangeType { kModify, kAdd, kRemove };

struct StackEntry {
  size_t index;
  SvgTag data;
  ChangeType type;  // conceptually: kModify -> index_change=false,
                    //               kAdd/kRemove -> index_change=true
};

class UndoRedoManager {
 public:
  // snapshots shape at index before modification, so undo can restore it.
  void RecordModify(size_t index, const SvgTag& oldData);
  // snapshots shape at index before removal, so undo can re‑insert it.
  void RecordRemove(size_t index, const SvgTag& removedData);
  // snapshots data added at index AFTER action , so undo can remove it.
  void RecordAdd(size_t index, const SvgTag& addedData);

  void Undo(std::vector<std::unique_ptr<Shape>>& shapes);
  void Redo(std::vector<std::unique_ptr<Shape>>& shapes);

 private:
  void Apply(std::stack<StackEntry>& from, std::stack<StackEntry>& to,
             std::vector<std::unique_ptr<Shape>>& shapes);

  std::stack<StackEntry> undo_stack_;
  std::stack<StackEntry> redo_stack_;
};

#endif  // CANVAS_UNDO_UNDOREDO_H_
