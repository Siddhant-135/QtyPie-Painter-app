# MICRO VECTOR EDITOR (SVG)
## COP 290 ASSIGNMENT 1 by Siddhant Agrawal (2024CS50469)

This is a graphic editor app that takes input and generates output in SVG Format.

### Requirements

- CMake (>= 3.25)
- A C++ compiler (on macOS: Xcode Command Line Tools)
- Qt **Widgets** (Qt6 or Qt5)

### Build and run

From the project folder, a single command configures **and** builds:

```bash
cmake --workflow --preset default
./build/cop290_assignment_1
```

> **Note:** This uses a CMake Workflow Preset defined in `CMakePresets.json` (requires CMake ≥ 3.25).
> If you have an older CMake, you can still build with two commands:
> ```bash
> cmake -S . -B build
> cmake --build build
> ```

### How to Use
#### TOP BAR
contains New (create a blank new file) Open (open existing .xml file) Save, Save As, Close (which also prompts you to save before you leave), Undo, Redo and Clear (deletes the topmost layer / shape in the canvas)

#### SHAPE FEATURES
- Has the option for various shapes (Square, circle, rectangle, Hexagon, rounded rectangle, Line) inplemented as drawings within a bounding box. Clicking within the bounding box of an object selects it. Then you can change different features of it. 
- Sketch allows you to draw freehand. When you click it and it changes colour, the tool has been selected by you, and you can drag it over your screen to draw. You need to click the button again or click another button to deselect it.
- TextBox appears as a rectangle on clicking it. you can select, resize and move it around with 

## CODE STRUCTURE

```
cop290/
├── main.cpp
├── CMakeLists.txt
├── README.md
├── SVG_README.md
├── svg_explanation.md
├── canvas/
│   ├── core/
│   │   ├── mycanvas.h
│   │   ├── mycanvas_shapes.cpp
│   │   └── mycanvas_render.cpp
│   ├── input/
│   │   ├── mycanvas_mouse.cpp
│   │   └── mycanvas_clipboard.cpp
│   └── undo/
│       ├── undoredo.h
│       └── undoredo.cpp
├── shapes/
│   ├── shapes.h
│   ├── base/
│   │   ├── shape.h
│   │   ├── shape.cpp
│   │   └── shape_resize.cpp
│   ├── uniform/
│   │   ├── square.h / square.cpp
│   │   ├── circle.h / circle.cpp
│   │   └── hexagon.h / hexagon.cpp
│   ├── rectangular/
│   │   ├── rectangle.h / rectangle.cpp
│   │   ├── rounded_rectangle.h / rounded_rectangle.cpp
│   │   └── text_box.h / text_box.cpp
│   └── freeform/
│       ├── line.h / line.cpp
│       └── polyline.h / polyline.cpp
├── svg/
│   ├── test_input.xml
│   ├── parser/
│   │   ├── Svg2Data.h
│   │   ├── Svg2Data_File.cpp
│   │   └── Svg2Data_Xml.cpp
│   ├── import/
│   │   ├── Data2Vec.h / Data2Vec.cpp
│   │   └── Data2Shape.h / Data2Shape.cpp
│   ├── export/
│   │   ├── Shape2Data.h / Shape2Data.cpp
│   │   └── Vec2Svg.h / Vec2Svg.cpp
│   └── registry/
│       ├── shape_registry.h / shape_registry.cpp
│       ├── shape_loaders_basic.cpp
│       └── shape_loaders_complex.cpp
└── ui/
    ├── main/
    │   ├── mainwidget.h
    │   ├── mainwidget.cpp
    │   └── mainwidget_file.cpp
    ├── panels/
    │   ├── colourpanel.h / colourpanel.cpp
    │   ├── controlpanel.h / controlpanel.cpp
    │   └── shapepanel.h / shapepanel.cpp
    └── menus/
        ├── floatingmenu.h
        └── floatingmenu.cpp
```

---

### Top-Level Files

- **`main.cpp`** — Application entry point. Creates a `QApplication`, instantiates the `MainWidget` window, shows it, and enters the Qt event loop.
- **`CMakeLists.txt`** — CMake build configuration (C++17, Qt6 Widgets). Lists all source files, enables AUTOMOC/AUTOUIC, and sets include directories.
- **`README.md`** — This file.
- **`SVG_README.md`** — Documentation of the SVG parser module: FSM states, attribute dictionary schema, usage examples, and instructions for adding new shapes.
- **`svg_explanation.md`** — High-level architectural explanation of the SVG load/save pipelines, with per-file responsibility summaries.

---

### FOLDER `canvas/` — Drawing Surface & Undo System

The central drawing canvas: a `QWidget` subclass (`MyCanvas`) that holds the shape vector, handles user interaction, and manages undo/redo.

#### `canvas/core/` — Canvas Core (declaration, shape management, rendering)

- **`mycanvas.h`** — Declares `MyCanvas` (QWidget subclass): the drawing surface holding the shape vector, selection state, undo/redo manager, clipboard, and freehand-drawing state.
- **`mycanvas_shapes.cpp`** — Implements shape-management methods: `AddShape`, `RemoveLastShape`, `ApplyColourSpec`, `ApplyFont`, `ApplyFontSize`, `EditSelectedText`, `GetShapes`, `SetFreehandMode`, `Undo`, `Redo`, and `ClearAll`.
- **`mycanvas_render.cpp`** — Implements `paintEvent`: iterates all shapes calling `Draw`, draws selection bbox/handles for the selected shape, and renders the in-progress freehand polyline.

#### `canvas/input/` — User Input Handling (mouse, clipboard)

- **`mycanvas_mouse.cpp`** — Implements mouse handlers (`mousePressEvent`, `mouseMoveEvent`, `mouseReleaseEvent`): shape selection, drag-move, handle-resize, right-click context menu, and freehand polyline drawing.
- **`mycanvas_clipboard.cpp`** — Implements clipboard and context-menu methods: `ShowFloatingMenu`, `CopySelected`, `DeleteSelected`, `CutSelected`, and `PasteClipboard`, using SVG-based serialisation.

#### `canvas/undo/` — Undo/Redo System

- **`undoredo.h`** — Declares `UndoRedoManager`, `ChangeType` enum (`kModify`/`kAdd`/`kRemove`), and `StackEntry` struct implementing a stack-based undo/redo system using serialised `SvgTag` snapshots.
- **`undoredo.cpp`** — Implements `RecordModify`/`RecordAdd`/`RecordRemove` and the symmetric `Apply` method that swaps shapes between undo/redo stacks via `Data2Shape`/`Shape2Data` conversion.

---

### FOLDER `shapes/` — Shape Hierarchy

All drawable shapes, inheriting from the abstract `Shape` base class. Each concrete shape lives in a subfolder grouped by resize behaviour.

- **`shapes.h`** — Aggregate header: includes every concrete shape header for convenient single-include access.

#### `shapes/base/` — Abstract Base Class

- **`shape.h`** — Declares the abstract `Shape` base class with bounding-box fields, fill/stroke colours, and virtual methods for drawing, hit-testing, selection, movement, handle resize, and bbox-change notification.
- **`shape.cpp`** — Implements default methods: `Draw`, `Select`/`Deselect`, `Move`, `IsSelected`, `HandleHitTest` (4-handle hit test), `ResizeFromHandle` (independent-axis resize), and `OnBboxChanged`.
- **`shape_resize.cpp`** — Implements `BoolResizeEqually`: a static helper that constrains a handle drag to maintain a square aspect ratio (used by `Square`, `Circle`, `Hexagon`).

#### `shapes/uniform/` — Shapes with Equal-Aspect Resize (Square, Circle, Hexagon)

- **`square.h / square.cpp`** — `Square` class: draws a filled rect, uses equal-aspect resize via `BoolResizeEqually`, syncs `x`/`y`/`side` from bbox.
- **`circle.h / circle.cpp`** — `Circle` class: draws an ellipse, uses equal-aspect resize, recomputes centre and radius from bbox.
- **`hexagon.h / hexagon.cpp`** — `Hexagon` class: draws a regular 6-vertex polygon, uses equal-aspect resize, recomputes centre/radius from bbox.

#### `shapes/rectangular/` — Axis-Aligned Rectangular Shapes

- **`rectangle.h / rectangle.cpp`** — `Rectangle` class: draws a filled rect with stroke, syncs `x`/`y`/`width`/`height` from the base bbox.
- **`rounded_rectangle.h / rounded_rectangle.cpp`** — `RoundedRectangle` class (extends `Rectangle`): adds a corner-radius handle at a draggable offset, clamps the radius on resize.
- **`text_box.h / text_box.cpp`** — `TextBox` class (extends `Rectangle`): renders text inside a transparent rectangle using a configured font/size. Text editing is handled via the canvas right-click menu.

#### `shapes/freeform/` — Free-Form Shapes (Line, Polyline)

- **`line.h / line.cpp`** — `Line` class: draws a line segment between two endpoints, supports two-endpoint handle hit-test and handle dragging.
- **`polyline.h / polyline.cpp`** — `Polyline` class: stores points as centre-relative offsets with scale factors, reconstructs world points for drawing, rescales to fit the new bbox on resize.

---

### FOLDER `svg/` — SVG Import/Export Pipeline

This folder contains implementations of interconversion between Three representations of our data
1. SVG: As an SVG File
2. DATA: As a 2D Dictionary, first layer of shape tags -> dictionary containing its attributes, and within the attributes dictionary, all its parameters -> string of value
3. SHAPE: As an Object of the Shape class, initialised with the values from dictionary (or default when not present)
4. VECTOR: a vector of shape objects which is the API endpoint actually used by the rest of the code to do its stuff. 

#### `svg/parser/` — XML → Data (SVG File Parsing)

- **`Svg2Data.h`** — Declares data structures `SvgTag` (tag name + attribute map), `SvgData` (canvas width/height + element list), and the `SvgParser` class with `ParseFile` and `ParseXml` methods.
- **`Svg2Data_File.cpp`** — Implements `ParseFile`: reads an XML file into a string, calls `ParseXml`, extracts `<svg>` width/height, and collects all other tags into `SvgData`.
- **`Svg2Data_Xml.cpp`** — Implements `ParseXml`: a character-by-character FSM (states: FIND_NEXT, GET_NAME, GET_KEY, GET_VAL) that parses XML tags and quoted attributes, invoking a callback per tag.

#### `svg/import/` — Data → Shapes (Constructing Shape Objects)

- **`Data2Vec.h / Data2Vec.cpp`** — `Data2Vec` class: converts an `SvgData` (list of `SvgTag`s) into a `vector<unique_ptr<Shape>>` by delegating each tag to `Data2Shape`.
- **`Data2Shape.h / Data2Shape.cpp`** — `Data2Shape` class: looks up a tag name in `ShapeRegistry`, calls the factory + inflater, parses common attributes (fill/stroke/stroke-width), and returns a single `Shape`.

#### `svg/export/` — Shapes → Data → SVG (Serialisation & File Writing)

- **`Shape2Data.h / Shape2Data.cpp`** — `Shape2Data` class: converts a `Shape` into an `SvgTag` using `dynamic_cast` chains to serialise each concrete shape type with appropriate SVG attributes.
- **`Vec2Svg.h / Vec2Svg.cpp`** — `Vec2Svg` class: writes a `vector<Shape>` to an SVG/XML file on disk, emitting `<svg>` header and per-shape elements (`<rect>`, `<circle>`, `<line>`, `<polyline>`, `<polygon>`, `<textbox>`).

#### `svg/registry/` — Shape Factory Registry

- **`shape_registry.h / shape_registry.cpp`** — Declares `ShapeRegistry` class, `ShapeEntry` struct (uiLabel, svgTag, factory, inflater), and type aliases. `GetEntries()` returns the static registry mapping SVG tags to constructors and attribute loaders.
- **`shape_loaders_basic.cpp`** — Implements loader/inflater functions for simple shapes: `LoadCircle`, `LoadRectangle`, `LoadSquare`, `LoadLine`, and `LoadPolyline`.
- **`shape_loaders_complex.cpp`** — Implements loader/inflater functions for complex shapes: `LoadTextBox` (parses text/font attributes), `LoadRoundedRectangle` (parses `rx` for corner radius), and `LoadHexagon` (reconstructs centre/radius from parsed polygon vertices).

#### `svg/test_input.xml`

- Sample SVG/XML test file containing rectangles, circles, a line, polylines (freehand strokes), and a rounded rectangle for manual import testing.

---

### FOLDER `ui/` — User Interface Widgets

All Qt widget classes that compose the application's user interface.

#### `ui/main/` — Main Application Window

- **`mainwidget.h`** — Declares `MainWidget` (top-level QWidget) holding pointers to `MyCanvas`, `ColourRibbon`, `ControlPanel`, `ShapePanel`, and file-management state.
- **`mainwidget.cpp`** — Implements the `MainWidget` constructor: creates and styles all child widgets, wires signals/slots (shape creation, colour changes, undo/redo, file ops, sketch mode), and assembles the vertical layout.
- **`mainwidget_file.cpp`** — Implements file I/O methods: `NewFile` (with save prompt), `OpenFile` (parse SVG → load shapes), `Save`, `SaveToFile`, `SaveAs` (via `QFileDialog`), and `CloseFile`.

#### `ui/panels/` — Side/Top Panel Widgets

- **`colourpanel.h / colourpanel.cpp`** — `ColourRibbon` widget: fill/stroke colour pickers, stroke-width spinner, font-family combo, and font-size spinner, with signals for each change.
- **`controlpanel.h / controlpanel.cpp`** — `ControlPanel` widget: eight buttons (New, Open, Save, Save As, Close, Undo, Redo, Clear) in a horizontal layout, each wired to a corresponding signal.
- **`shapepanel.h / shapepanel.cpp`** — `ShapePanel` widget: dynamically creates a button per registered shape (from `ShapeRegistry`), plus a toggleable "Sketch" button for freehand mode. Emits `ShapeSelected` and `SketchToggled` signals.

#### `ui/menus/` — Popup / Context Menus

- **`floatingmenu.h / floatingmenu.cpp`** — `FloatingMenu` popup widget: a right-click context menu with four buttons (Copy, Cut, Delete, Paste), each emitting its signal and hiding the menu on click.
