# SVG Module Explanation

This document explains how the `svg/` folder converts between SVG files and in‑memory shapes.

## Overview of the pipeline

There are two conversion directions:

**Load (SVG → Shapes):**
1. `SvgParser::parseSvgFile()` parses the SVG/XML into a `SvgData` object (width/height + list of `SvgTag` elements).  
2. `Data2Vec::convertToShapes()` iterates tags and calls `Data2Shape::convert()` for each tag.  
3. `Data2Shape::convert()` uses `ShapeRegistry` to map `SvgTag.name` to a factory + loader and builds a `Shape`.

**Save (Shapes → SVG):**
1. `Shape2Data::convert()` converts each `Shape` into a `SvgTag` with attributes.  
2. `Vec2Svg::saveShapesToSvgFile()` writes an SVG header and serializes all tags.

---

## File-by-file responsibilities

### svg/Svg2Data.h + svg_parser.cpp
- Defines `SvgTag` (tag name + attribute map) and `SvgData` (canvas size + list of tags).
- `SvgParser::parseSvgFile()` reads the XML, extracts tags and attributes, and populates `SvgData`.

### svg/Data2Vec.h + Data2Vec.cpp
- Converts `SvgData` → `std::vector<std::unique_ptr<Shape>>`.
- Delegates each tag to `Data2Shape::convert()`.

### svg/Data2Shape.h + Data2Shape.cpp
- Converts a single `SvgTag` → `Shape`.
- Looks up the tag in `ShapeRegistry` and calls the corresponding factory + loader.

### svg/shape_registry.h + shape_registry.cpp
- Central registry of all supported shapes.
- Each entry contains:
  - `uiLabel` (button label in UI)
  - `svgTag` (tag name used in SVG)
  - `create` (factory function)
  - `load` (inflate attributes into the Shape)
- This is the single source of truth for SVG tag → Shape mapping.

### svg/shape_loaders_1.cpp
- Implements the `load*` functions used by `ShapeRegistry`.
- Example: `loadTextBox()` reads `x/y/width/height/text/font-family/font-size`.

### svg/Shape2Data.h + Shape2Data.cpp
- Converts a `Shape` into an equivalent `SvgTag`.
- Serializes properties into SVG attributes.
- Example: `TextBox` writes `text`, `font-family`, `font-size`.

### svg/Vec2Svg.h + Vec2Svg.cpp
- Writes the SVG file.
- Builds `<svg ...>` header, then serializes tags from `Shape2Data`.

---

## Example flow (TextBox)

**Saving:**  
`TextBox` → `Shape2Data::convert()` → `<textbox x="..." y="..." width="..." height="..." text="..." font-family="..." font-size="...">`  
→ `Vec2Svg::saveShapesToSvgFile()` writes file.

**Loading:**  
`SvgParser::parseSvgFile()` → `Data2Vec::convertToShapes()`  
→ `Data2Shape::convert()` → `ShapeRegistry::loadTextBox()`  
→ `TextBox` object with text/font applied.

---

## Adding a new shape

1. Add a new `Shape` class (e.g., in `shapes/`).
2. Register it in `ShapeRegistry::getRegistry()` with:
   - `uiLabel`
   - `svgTag`
   - factory
   - loader
3. Implement `ShapeRegistry::loadYourShape()` in `shape_loaders_1.cpp`.
4. Add serialization logic to `Shape2Data::convert()`.
5. (Optional) Add UI controls if needed.

---

## Notes
- Parsing is intentionally lightweight; it only reads attributes needed for supported shapes.
- Unsupported SVG tags are ignored (no registry entry).