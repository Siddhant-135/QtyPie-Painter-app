#include "svg_parser.h"
#include <fstream>
#include <iterator>
#include <cctype>

// -- Helpers: safe extraction from raw temp_map --
static int mapInt(const std::unordered_map<std::string, std::string>& m,
                  const std::string& key, int fallback = 0) {
    auto it = m.find(key);
    if (it == m.end() || it->second.empty()) return fallback;
    return std::stoi(it->second);
}
static std::string mapStr(const std::unordered_map<std::string, std::string>& m,
                          const std::string& key) {
    auto it = m.find(key);
    return (it == m.end()) ? std::string{} : it->second;
}

// -- Finalize: convert raw temp_map into a ParsedShape or svg dims --
static void finalize(const std::string& tag,
                     const std::unordered_map<std::string, std::string>& temp_map,
                     SvgParseResult& result) {
    if (tag == "svg") {
        result.svg_width  = mapInt(temp_map, "width");
        result.svg_height = mapInt(temp_map, "height");
        return;
    }
    ParsedShape shape;
    shape.tag = tag;
    if (tag == "circle") {
        shape.attrs["cx"] = mapInt(temp_map, "cx");
        shape.attrs["cy"] = mapInt(temp_map, "cy");
        shape.attrs["r"]  = mapInt(temp_map, "r");
    } else if (tag == "rect") {
        shape.attrs["x"]      = mapInt(temp_map, "x");
        shape.attrs["y"]      = mapInt(temp_map, "y");
        shape.attrs["width"]  = mapInt(temp_map, "width");
        shape.attrs["height"] = mapInt(temp_map, "height");
    } else if (tag == "line") {
        shape.attrs["x1"] = mapInt(temp_map, "x1");
        shape.attrs["y1"] = mapInt(temp_map, "y1");
        shape.attrs["x2"] = mapInt(temp_map, "x2");
        shape.attrs["y2"] = mapInt(temp_map, "y2");
    } else {
        return; // unknown tag — skip
    }
    // Visual params stored as raw strings; converted to QColor in svg_convert
    shape.attrs["stroke-width"] = mapInt(temp_map, "stroke-width", 0);
    shape.attrs["fill"]   = mapStr(temp_map, "fill");
    shape.attrs["stroke"] = mapStr(temp_map, "stroke");
    // Rotation deferred — store raw transform string
    std::string xform = mapStr(temp_map, "transform");
    if (!xform.empty()) shape.attrs["transform"] = std::move(xform);
    result.shapes.push_back(std::move(shape));
}

// -- Character-by-character FSM --
SvgParseResult parseSvgString(const std::string& svg_string) {
    enum State { SEEK_TAG, READ_TAG_NAME, SEEK_ATTR, READ_ATTR_KEY, READ_ATTR_VALUE };
    SvgParseResult result;
    State state = SEEK_TAG;
    int A = 0;
    std::string word_buffer, current_tag, current_key;
    std::unordered_map<std::string, std::string> temp_map;

    for (char ch : svg_string) {
        if (ch == '<') {
            temp_map.clear(); word_buffer.clear();
            state = READ_TAG_NAME; continue;
        }
        switch (state) {
        case READ_TAG_NAME:
            if (word_buffer.empty() && (ch == '/' || ch == '?' || ch == '!')) {
                state = SEEK_TAG;
            } else if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '>') {
                current_tag = word_buffer; word_buffer.clear();
                if (ch == '>') { finalize(current_tag, temp_map, result); state = SEEK_TAG; }
                else state = SEEK_ATTR;
            } else { word_buffer += ch; }
            break;
        case SEEK_ATTR:
            if (ch == '>') { finalize(current_tag, temp_map, result); state = SEEK_TAG; }
            else if (ch == '/') { /* self-closing, wait for > */ }
            else if (std::isalpha(static_cast<unsigned char>(ch)) || ch == '-') {
                word_buffer.clear(); word_buffer += ch; state = READ_ATTR_KEY;
            }
            break;
        case READ_ATTR_KEY:
            if (ch == '=') { current_key = word_buffer; word_buffer.clear(); state = READ_ATTR_VALUE; A = 0; }
            else { word_buffer += ch; }
            break;
        case READ_ATTR_VALUE:
            if (ch == '"') { A ^= 1; if (A == 0) { temp_map[current_key] = word_buffer; word_buffer.clear(); state = SEEK_ATTR; } }
            else if (A == 1) { word_buffer += ch; }
            break;
        case SEEK_TAG: default: break;
        }
    }
    return result;
}

// -- File I/O --
SvgParseResult parseSvgFile(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    return parseSvgString(contents);
}
