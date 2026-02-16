# MICRO VECTOR EDITOR (SVG)
## COP 290 ASSIGNMENT 1 by Siddhant Agrawal (2024CS50469)

This is a graphic editor app that takes input and generates output in SVG Format.

### Requirements

- CMake (>= 3.16)
- A C++ compiler (on macOS: Xcode Command Line Tools)
- Qt **Widgets** (Qt6 or Qt5)

### Build and run

From the project folder:

```bash
cmake -S . -B build
cmake --build build
./build/cop290_assignment_1
```

### How to Use
#### TOP BAR
contains New (create a blank new file) Open (open existing .xml file) Save, Save As, Close (which also prompts you to save before you leave), Undo, Redo and Clear (deletes the topmost layer / shape in the canvas)

#### SHAPE FEATURES
- Has the option for various shapes (Square, circle, rectangle, Hexagon, rounded rectangle, Line) inplemented as drawings within a bounding box. Clicking within the bounding box of an object selects it. Then you can change different features of it. 
- Sketch allows you to draw freehand. When you click it and it changes colour, the tool has been selected by you, and you can drag it over your screen to draw. You need to click the button again or click another button to deselect it.
- TextBox is implemented as a child of rectangle, with a 

## CODE STRUCTURE

### FOLDER SVG
This folder contains implementations of interconversion between Three representations of our data
1. SVG: As an SVG File
2. DATA: As a 2D Dictionary, first layer of shape tags -> dictionary containing its attributes, and within the attributes dictionary, all its parameters -> string of value
3. SHAPE: As an Object of the Shape class, initialised with the values from dictionary (or default when not present)
4. VECTOR: a vector of shape objects which is the API endpoint actually used by the rest of the code to do its stuff. 

### FOLDER UI


