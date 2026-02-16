#include "undoredo.h"

#include "../svg/Data2Shape.h"
#include "../svg/Shape2Data.h"

// ── Recording ───────────────────────────────────────────────

void UndoRedoManager::RecordModify(size_t index, const SvgTag& oldData) {
  undo_stack_.push({index, oldData, ChangeType::MODIFY});
  while (!redo_stack_.empty()) redo_stack_.pop();  // new action clears redo
}

void UndoRedoManager::RecordRemove(size_t index, const SvgTag& removedData) {
  // Undo should re‑ADD the removed shape → entry type = ADD
  undo_stack_.push({index, removedData, ChangeType::ADD});
  while (!redo_stack_.empty()) redo_stack_.pop();
}

void UndoRedoManager::RecordAdd(size_t index, const SvgTag& addedData) {
  // Undo should REMOVE the added shape → entry type = REMOVE
  undo_stack_.push({index, addedData, ChangeType::REMOVE});
  while (!redo_stack_.empty()) redo_stack_.pop();
}

// ── Undo / Redo ─────────────────────────────────────────────

void UndoRedoManager::Undo(std::vector<std::unique_ptr<Shape>>& shapes) {
  Apply(undo_stack_, redo_stack_, shapes);
}

void UndoRedoManager::Redo(std::vector<std::unique_ptr<Shape>>& shapes) {
  Apply(redo_stack_, undo_stack_, shapes);
}

// ── Core apply (identical logic for both directions) ────────

void UndoRedoManager::Apply(std::stack<StackEntry>& from,
                            std::stack<StackEntry>& to,
                            std::vector<std::unique_ptr<Shape>>& shapes) {
  if (from.empty()) return;
  StackEntry entry = std::move(from.top());
  from.pop();

  switch (entry.type) {
    case ChangeType::MODIFY: {
      // Swap: serialize current shape, push inverse, replace with stored
      if (entry.index >= shapes.size()) return;
      SvgTag current = Shape2Data::Convert(*shapes[entry.index]);
      to.push({entry.index, std::move(current), ChangeType::MODIFY});
      shapes[entry.index] = Data2Shape::Convert(entry.data);
      break;
    }
    case ChangeType::ADD: {
      // Insert shape at index; inverse = REMOVE
      auto shape = Data2Shape::Convert(entry.data);
      if (!shape) return;
      size_t idx = std::min(entry.index, shapes.size());
      shapes.insert(shapes.begin() + static_cast<long>(idx), std::move(shape));
      to.push({idx, entry.data, ChangeType::REMOVE});
      break;
    }
    case ChangeType::REMOVE: {
      // Remove shape at index; inverse = ADD
      if (entry.index >= shapes.size()) return;
      SvgTag removed = Shape2Data::Convert(*shapes[entry.index]);
      shapes.erase(shapes.begin() + static_cast<long>(entry.index));
      to.push({entry.index, std::move(removed), ChangeType::ADD});
      break;
    }
  }
}
