# Qt Rectangle Demo (CMake + Widgets)

This is a tiny beginner Qt app.

- Click **Rectangle** to draw a fixed-size solid rectangle.
- Click **Clear** to remove it.

## Requirements

- CMake (>= 3.16)
- A C++ compiler (on macOS: Xcode Command Line Tools)
- Qt **Widgets** (Qt6 or Qt5)

## Build + run

From the project folder:

```bash
cmake -S . -B build
cmake --build build
./build/cop290_assignment_1
```

## If CMake can’t find Qt (common on macOS)

You need to point CMake at your Qt install prefix.

### If you installed Qt with Homebrew

Qt6 is often installed under:

```bash
brew --prefix qt
```

Then build like:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$(brew --prefix qt)"
cmake --build build
./build/cop290_assignment_1
```

### If you installed Qt from the Qt Online Installer

Set `CMAKE_PREFIX_PATH` to the folder that contains `lib/cmake/Qt6` (or `Qt5`), for example:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH="$HOME/Qt/6.6.0/macos"
cmake --build build
./build/cop290_assignment_1
```

## What to read in the code

All code is in `main.cpp`:

- `Canvas::paintEvent(...)` draws the rectangle using `QPainter`
- `update()` triggers repainting after button clicks

