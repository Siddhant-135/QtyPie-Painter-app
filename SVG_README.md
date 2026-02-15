# SVG Parser Module

Standalone SVG parser that reads an `.xml` file containing standard SVG markup and produces `Shape` objects compatible with `MyCanvas`.

## File Structure

| File | Purpose |
|---|---|
| `svg_parser.h` | Shared types: `AttrValue`, `ParsedShape`, `SvgParseResult` |
| `svg_parser.cpp` | Single-pass FSM that converts an SVG string into a `vector<ParsedShape>` |
| `svg_convert.h` | Declares `parseColor` and `toShapes` |
| `svg_convert.cpp` | SVG-to-shapes.h dictionary, colour parser, `ParsedShape` to `unique_ptr<Shape>` conversion |
| `test_input.xml` | Sample SVG with rects, circles, and a line for manual testing |

## FSM States

```
SEEK_TAG  ──'<'──>  READ_TAG_NAME  ──space──>  SEEK_ATTR  ──alpha──>  READ_ATTR_KEY  ──'='──>  READ_ATTR_VALUE
                         │                        │                                                  │
                        '>'                      '>'                                           closing '"'
                         │                        │                                                  │
                         v                        v                                                  v
                    finalize()              finalize()                                          SEEK_ATTR
```

Tags starting with `/`, `?`, or `!` are skipped back to `SEEK_TAG`. Self-closing `/>` is handled by ignoring `/` in `SEEK_ATTR`.

## Dictionary Schema

The dictionary in `svg_convert.cpp` maps SVG attribute names to `shapes.h` field names. It is manually typed and must be updated when shapes change.

```
SVG tag  | SVG attr      -> shapes.h field
---------|----------------------------------
circle   | cx            -> cx
         | cy            -> cy
         | r             -> radius
rect     | x             -> x
         | y             -> y
         | width         -> w
         | height        -> h
line     | x1            -> x1
         | y1            -> y1
         | x2            -> x2
         | y2            -> y2
(common) | fill          -> fillColour
         | stroke        -> strokeColour
         | stroke-width  -> strokeWidth
```

## Usage

```cpp
#include "svg_parser.h"
#include "svg_convert.h"

// 1. Parse the file
auto result = parseSvgFile("test_input.xml");

// 2. Convert to Shape objects
auto shapes = toShapes(result);

// 3. Size the canvas (1.2x scaling)
canvas->setFixedSize(result.svg_width * 1.2, result.svg_height * 1.2);

// 4. Add shapes to canvas
for (auto& s : shapes)
    canvas->addshape(std::move(s));
```

## Adding New Shapes

1. Define the struct in `shapes.h`.
2. Add an `else if` branch in `finalize()` (`svg_parser.cpp`) to extract the SVG-standard attributes.
3. Add an `else if` branch in `toShapes()` (`svg_convert.cpp`) to construct the new struct, mapping SVG attr names to `shapes.h` field names.
4. Update the dictionary comment in `svg_convert.cpp`.

## Notes

- **Rotation**: The `transform` attribute is captured as a raw string but not yet processed. A future `parseRotation` function will extract angle and center from `"rotate(deg x y)"`.
- **Colour**: `parseColor` handles `#RRGGBB` hex and 20 named SVG colours. `"none"` maps to `Qt::transparent`. Unrecognised strings fall back to black.
- **Performance**: Single pass, O(n), no recursion, no regex.
