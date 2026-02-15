#include "svg_parser.h"
#include <fstream>
#include <iterator>
#include <cctype>

static int getInt(const std::unordered_map<std::string, std::string>& map, const std::string& key, int fallback = 0) {
    auto it = map.find(key);
    if (it == map.end() || it->second.empty()) return fallback;
    return std::stoi(it->second);
}

static std::string getString(const std::unordered_map<std::string, std::string>& map, const std::string& key) {
    auto it = map.find(key);
    return (it == map.end()) ? std::string{} : it->second;
}

static void processElement(const std::string& tag, const std::unordered_map<std::string, std::string>& rawAttrs, SvgDocument& doc) {
    if (tag == "svg") {
        doc.width = getInt(rawAttrs, "width");
        doc.height = getInt(rawAttrs, "height");
        return;
    }
    RawShapeData shape;
    shape.tagName = tag;
    if (tag == "circle") {
        shape.attributes["cx"] = getInt(rawAttrs, "cx");
        shape.attributes["cy"] = getInt(rawAttrs, "cy");
        shape.attributes["r"] = getInt(rawAttrs, "r");
    } else if (tag == "rect") {
        shape.attributes["x"] = getInt(rawAttrs, "x");
        shape.attributes["y"] = getInt(rawAttrs, "y");
        shape.attributes["width"] = getInt(rawAttrs, "width");
        shape.attributes["height"] = getInt(rawAttrs, "height");
    } else if (tag == "line") {
        shape.attributes["x1"] = getInt(rawAttrs, "x1");
        shape.attributes["y1"] = getInt(rawAttrs, "y1");
        shape.attributes["x2"] = getInt(rawAttrs, "x2");
        shape.attributes["y2"] = getInt(rawAttrs, "y2");
    } else return;

    shape.attributes["stroke-width"] = getInt(rawAttrs, "stroke-width", 0);
    shape.attributes["fill"] = getString(rawAttrs, "fill");
    shape.attributes["stroke"] = getString(rawAttrs, "stroke");
    
    std::string xform = getString(rawAttrs, "transform");
    if (!xform.empty()) shape.attributes["transform"] = std::move(xform);
    doc.shapes.push_back(std::move(shape));
}

SvgDocument parseSvgString(const std::string& xmlContent) {
    enum State { FIND_TAG, READ_NAME, FIND_ATTR, READ_KEY, READ_VALUE };
    SvgDocument doc;
    State state = FIND_TAG;
    bool insideQuotes = false;
    std::string buffer, currentTag, currentKey;
    std::unordered_map<std::string, std::string> attributes;

    for (char c : xmlContent) {
        if (c == '<') {
            attributes.clear(); 
            buffer.clear();
            state = READ_NAME; 
            continue;
        }
        switch (state) {
        case READ_NAME:
            if (buffer.empty() && (c == '/' || c == '?' || c == '!')) {
                state = FIND_TAG;
            } else if (std::isspace(c) || c == '>') {
                currentTag = buffer; buffer.clear();
                if (c == '>') { processElement(currentTag, attributes, doc); state = FIND_TAG; }
                else state = FIND_ATTR;
            } else buffer += c;
            break;
        case FIND_ATTR:
            if (c == '>') { processElement(currentTag, attributes, doc); state = FIND_TAG; }
            else if (std::isalpha(static_cast<unsigned char>(c)) || c == '-') {
                buffer.clear(); buffer += c; state = READ_KEY;
            }
            break;
        case READ_KEY:
            if (c == '=') { currentKey = buffer; buffer.clear(); state = READ_VALUE; insideQuotes = false; }
            else buffer += c;
            break;
        case READ_VALUE:
            if (c == '"') {
                insideQuotes = !insideQuotes;
                if (!insideQuotes) { attributes[currentKey] = buffer; buffer.clear(); state = FIND_ATTR; }
            } else if (insideQuotes) buffer += c;
            break;
        default: break;
        }
    }
    return doc;
}

SvgDocument parseSvgFile(const std::string& filePath) {
    std::ifstream file(filePath);
    return parseSvgString(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
}