#include "undoredo.h"

#include "../svg/Data2Shape.h"
#include "../svg/Shape2Data.h"

// ── Recording ───────────────────────────────────────────────

void UndoRedoManager::recordModify(size_t index, const SvgTag& oldData) {
  undoStack.push({index, oldData, ChangeType::MODIFY});
  while (!redoStack.empty()) redoStack.pop();  // new action clears redo
}

void UndoRedoManager::recordRemove(size_t index, const SvgTag& removedData) {
  // Undo should re‑ADD the removed shape → entry type = ADD
  undoStack.push({index, removedData, ChangeType::ADD});
  while (!redoStack.empty()) redoStack.pop();
}

void UndoRedoManager::recordAdd(size_t index, const SvgTag& addedData) {
  // Undo should REMOVE the added shape → entry type = REMOVE
  undoStack.push({index, addedData, ChangeType::REMOVE});
  while (!redoStack.empty()) redoStack.pop();
}

// ── Undo / Redo ─────────────────────────────────────────────

void UndoRedoManager::undo(std::vector<std::unique_ptr<Shape>>& shapes) {
  apply(undoStack, redoStack, shapes);
}

void UndoRedoManager::redo(std::vector<std::unique_ptr<Shape>>& shapes) {
  apply(redoStack, undoStack, shapes);
}

// ── Core apply (identical logic for both directions) ────────

void UndoRedoManager::apply(std::stack<StackEntry>& from,
                            std::stack<StackEntry>& to,
                            std::vector<std::unique_ptr<Shape>>& shapes) {
  if (from.empty()) return;
  StackEntry entry = std::move(from.top());
  from.pop();

  switch (entry.type) {
    case ChangeType::MODIFY: {
      // Swap: serialize current shape, push inverse, replace with stored
      if (entry.index >= shapes.size()) return;
      SvgTag current = Shape2Data::convert(*shapes[entry.index]);
      to.push({entry.index, std::move(current), ChangeType::MODIFY});
      shapes[entry.index] = Data2Shape::convert(entry.data);
      break;
    }
    case ChangeType::ADD: {
      // Insert shape at index; inverse = REMOVE
      auto shape = Data2Shape::convert(entry.data);
      if (!shape) return;
      size_t idx = std::min(entry.index, shapes.size());
      shapes.insert(shapes.begin() + static_cast<long>(idx), std::move(shape));
      to.push({idx, entry.data, ChangeType::REMOVE});
      break;
    }
    case ChangeType::REMOVE: {
      // Remove shape at index; inverse = ADD
      if (entry.index >= shapes.size()) return;
      SvgTag removed = Shape2Data::convert(*shapes[entry.index]);
      shapes.erase(shapes.begin() + static_cast<long>(entry.index));
      to.push({entry.index, std::move(removed), ChangeType::ADD});
      break;
    }
  }
}
