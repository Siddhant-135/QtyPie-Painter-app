#include "svg_convert.h"
#include <QString>
#include <algorithm>
#include <cmath>

QColor stringToColor(const std::string& colorName) {
    if (colorName.empty()) return Qt::black;
    if (colorName[0] == '#') return QColor(QString::fromStdString(colorName));
    static const std::unordered_map<std::string, QColor> colorMap = {
        {"none", Qt::transparent}, {"black", Qt::black}, {"white", Qt::white},
        {"red", Qt::red}, {"green", Qt::darkGreen}, {"blue", Qt::blue},
        {"yellow", Qt::yellow}, {"cyan", Qt::cyan}, {"magenta", Qt::magenta},
        {"orange", QColor(255,165,0)}, {"purple", QColor(128,0,128)},
        {"gray", Qt::gray}, {"grey", Qt::gray}
    };
    auto it = colorMap.find(colorName);
    return (it != colorMap.end()) ? it->second : Qt::black;
}

static int getAttrInt(const RawShapeData& data, const std::string& key, int defaultValue = 0) {
    auto it = data.attributes.find(key);
    if (it == data.attributes.end()) return defaultValue;
    if (auto* val = std::get_if<int>(&it->second)) return *val;
    return defaultValue;
}

static QColor getAttrColor(const RawShapeData& data, const std::string& key, QColor defaultValue = Qt::black) {
    auto it = data.attributes.find(key);
    if (it == data.attributes.end()) return defaultValue;
    if (auto* val = std::get_if<QColor>(&it->second)) return *val;
    if (auto* val = std::get_if<std::string>(&it->second)) return stringToColor(*val);
    return defaultValue;
}

std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgDocument& svgData) {
    std::vector<std::unique_ptr<Shape>> createdShapes;
    for (const auto& raw : svgData.shapes) {
        std::unique_ptr<Shape> newShape;
        if (raw.tagName == "circle") {
            auto c = std::make_unique<Circle>();
            c->cx = getAttrInt(raw, "cx");
            c->cy = getAttrInt(raw, "cy");
            c->radius = getAttrInt(raw, "r");
            c->bbox_x = c->cx - c->radius;
            c->bbox_y = c->cy - c->radius;
            c->bbox_w = c->radius * 2;
            c->bbox_h = c->radius * 2;
            newShape = std::move(c);
        } else if (raw.tagName == "rect") {
            auto r = std::make_unique<Rectangle>();
            r->x = getAttrInt(raw, "x");
            r->y = getAttrInt(raw, "y");
            r->w = getAttrInt(raw, "width");
            r->h = getAttrInt(raw, "height");
            r->bbox_x = r->x; r->bbox_y = r->y;
            r->bbox_w = r->w; r->bbox_h = r->h;
            newShape = std::move(r);
        } else if (raw.tagName == "line") {
            auto l = std::make_unique<Line>();
            l->x1 = getAttrInt(raw, "x1"); l->y1 = getAttrInt(raw, "y1");
            l->x2 = getAttrInt(raw, "x2"); l->y2 = getAttrInt(raw, "y2");
            l->bbox_x = std::min(l->x1, l->x2);
            l->bbox_y = std::min(l->y1, l->y2);
            l->bbox_w = std::abs(l->x2 - l->x1);
            l->bbox_h = std::abs(l->y2 - l->y1);
            newShape = std::move(l);
        } else continue;

        newShape->fillColour = getAttrColor(raw, "fill", Qt::black);
        newShape->strokeColour = getAttrColor(raw, "stroke", Qt::transparent);
        newShape->strokeWidth = getAttrInt(raw, "stroke-width", 0);
        createdShapes.push_back(std::move(newShape));
    }
    return createdShapes;
}