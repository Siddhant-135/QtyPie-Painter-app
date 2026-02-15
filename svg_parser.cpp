#include "svg_parser.h"
#include <fstream>
#include <functional>
#include <cctype>

void parseXml(const std::string& data, std::function<void(std::string, AttrMap)> onElement) {
    enum State { FIND_TAG, TAG_NAME, ATTR_KEY, ATTR_VAL };
    State state = FIND_TAG;
    std::string buf, tag, key;
    AttrMap attrs;
    bool quoted = false;

    for (char c : data) {
        if (c == '<') { attrs.clear(); buf.clear(); state = TAG_NAME; continue; }
        if (state == TAG_NAME) {
            if (std::isspace(c) || c == '>') {
                tag = buf; buf.clear();
                if (c == '>') { onElement(tag, attrs); state = FIND_TAG; } else state = ATTR_KEY;
            } else buf += c;
        } else if (state == ATTR_KEY) {
            if (c == '>') { onElement(tag, attrs); state = FIND_TAG; }
            else if (c == '=') { key = buf; buf.clear(); state = ATTR_VAL; quoted = false; }
            else if (!std::isspace(c)) buf += c;
        } else if (state == ATTR_VAL) {
            if (c == '\"') {
                if (quoted) { attrs[key] = buf; buf.clear(); state = ATTR_KEY; }
                quoted = !quoted;
            } else if (quoted) buf += c;
        }
    }
}

SvgData parseSvgFile(const std::string& path) {
    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)), {});
    SvgData doc;
    parseXml(content, [&](std::string tag, AttrMap attrs) {
        if (tag == "svg") {
            if (attrs.count("width")) doc.width = std::stoi(attrs["width"]);
            if (attrs.count("height")) doc.height = std::stoi(attrs["height"]);
        } else doc.elements.push_back({tag, attrs});
    });
    return doc;
}