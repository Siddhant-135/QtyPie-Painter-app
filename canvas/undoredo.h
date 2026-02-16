#pragma once

#include <cstddef>
#include <memory>
#include <stack>
#include <vector>

#include "../svg/Svg2Data.h"

struct Shape;

// What this entry DOES when applied:
//   MODIFY  – replace shape at index with stored data (swap)
//   ADD     – insert stored data at index
//   REMOVE  – remove shape at index (data kept for inverse)
enum class ChangeType { MODIFY, ADD, REMOVE };

struct StackEntry {
  size_t index;
  SvgTag data;
  ChangeType type;         // conceptually: MODIFY ↔ index_change=false,
                           //               ADD/REMOVE ↔ index_change=true
};

class UndoRedoManager {
 public:
  // --- Recording helpers (called BEFORE the action for modify/remove,
  //                          AFTER the action for add) ---

  // Shape at `index` is about to be modified; snapshot its current state.
  void RecordModify(size_t index, const SvgTag& oldData);

  // Shape at `index` was just removed; store it so undo can re‑insert.
  void RecordRemove(size_t index, const SvgTag& removedData);

  // Shape was just added at `index`; store it so undo can delete it.
  void RecordAdd(size_t index, const SvgTag& addedData);

  // --- Apply undo / redo ---
  void Undo(std::vector<std::unique_ptr<Shape>>& shapes);
  void Redo(std::vector<std::unique_ptr<Shape>>& shapes);

 private:
  void Apply(std::stack<StackEntry>& from,
             std::stack<StackEntry>& to,
             std::vector<std::unique_ptr<Shape>>& shapes);

  std::stack<StackEntry> undo_stack_;
  std::stack<StackEntry> redo_stack_;
};
