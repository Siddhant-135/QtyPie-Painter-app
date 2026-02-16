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
  void recordModify(size_t index, const SvgTag& oldData);

  // Shape at `index` was just removed; store it so undo can re‑insert.
  void recordRemove(size_t index, const SvgTag& removedData);

  // Shape was just added at `index`; store it so undo can delete it.
  void recordAdd(size_t index, const SvgTag& addedData);

  // --- Apply undo / redo ---
  void undo(std::vector<std::unique_ptr<Shape>>& shapes);
  void redo(std::vector<std::unique_ptr<Shape>>& shapes);

 private:
  void apply(std::stack<StackEntry>& from,
             std::stack<StackEntry>& to,
             std::vector<std::unique_ptr<Shape>>& shapes);

  std::stack<StackEntry> undoStack;
  std::stack<StackEntry> redoStack;
};
