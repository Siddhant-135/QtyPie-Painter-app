#include "svg_convert.h"
#include <QString>
#include <algorithm>
#include <cmath>

// ============================================================================
// SVG → shapes.h name dictionary (manually typed, single source of truth)
// ============================================================================
//
//  SVG tag    | SVG attr       → shapes.h field
//  -----------|-----------------------------------
//  circle     | cx             → cx
//             | cy             → cy
//             | r              → radius
//  rect       | x              → x
//             | y              → y
//             | width          → w
//             | height         → h
//  line       | x1             → x1
//             | y1             → y1
//             | x2             → x2
//             | y2             → y2
//  (common)   | fill           → fillColour
//             | stroke         → strokeColour
//             | stroke-width   → strokeWidth
//

// ============================================================================
// parseColor — SVG colour string → QColor
// ============================================================================
QColor parseColor(const std::string& colorStr) {
    if (colorStr.empty()) return Qt::black;
    if (colorStr[0] == '#') return QColor(QString::fromStdString(colorStr));
    static const std::unordered_map<std::string, QColor> named = {
        {"none",    Qt::transparent},
        {"black",   QColor(0,0,0)},       {"white",   QColor(255,255,255)},
        {"red",     QColor(255,0,0)},      {"green",   QColor(0,128,0)},
        {"blue",    QColor(0,0,255)},      {"yellow",  QColor(255,255,0)},
        {"cyan",    QColor(0,255,255)},    {"magenta", QColor(255,0,255)},
        {"orange",  QColor(255,165,0)},    {"purple",  QColor(128,0,128)},
        {"lime",    QColor(0,255,0)},      {"maroon",  QColor(128,0,0)},
        {"navy",    QColor(0,0,128)},      {"olive",   QColor(128,128,0)},
        {"teal",    QColor(0,128,128)},    {"silver",  QColor(192,192,192)},
        {"gray",    QColor(128,128,128)},  {"grey",    QColor(128,128,128)},
        {"fuchsia", QColor(255,0,255)},    {"aqua",    QColor(0,255,255)},
    };
    auto it = named.find(colorStr);
    return (it != named.end()) ? it->second : Qt::black;
}

// ============================================================================
// Helpers — safe typed access into ParsedShape attrs
// ============================================================================
static int attrInt(const ParsedShape& s, const std::string& key, int fb = 0) {
    auto it = s.attrs.find(key);
    if (it == s.attrs.end()) return fb;
    if (auto* v = std::get_if<int>(&it->second)) return *v;
    return fb;
}
static QColor attrColor(const ParsedShape& s, const std::string& key, QColor fb = Qt::black) {
    auto it = s.attrs.find(key);
    if (it == s.attrs.end()) return fb;
    if (auto* v = std::get_if<QColor>(&it->second)) return *v;
    if (auto* v = std::get_if<std::string>(&it->second)) return parseColor(*v);
    return fb;
}

// ============================================================================
// toShapes — ParsedShape vector → unique_ptr<Shape> vector
// ============================================================================
// Follows the dictionary above: SVG attr names on the left, shapes.h field
// names on the right. bbox_* is recomputed so hit-testing works.

std::vector<std::unique_ptr<Shape>> toShapes(const SvgParseResult& result) {
    std::vector<std::unique_ptr<Shape>> out;
    out.reserve(result.shapes.size());

    for (const auto& ps : result.shapes) {
        std::unique_ptr<Shape> sp;

        if (ps.tag == "circle") {
            auto c    = std::make_unique<Circle>();
            c->cx     = attrInt(ps, "cx");        // cx → cx
            c->cy     = attrInt(ps, "cy");        // cy → cy
            c->radius = attrInt(ps, "r");         // r  → radius
            c->bbox_x = c->cx - c->radius;
            c->bbox_y = c->cy - c->radius;
            c->bbox_w = c->radius * 2;
            c->bbox_h = c->radius * 2;
            sp = std::move(c);

        } else if (ps.tag == "rect") {
            auto r = std::make_unique<Rectangle>();
            r->x   = attrInt(ps, "x");            // x      → x
            r->y   = attrInt(ps, "y");            // y      → y
            r->w   = attrInt(ps, "width");        // width  → w
            r->h   = attrInt(ps, "height");       // height → h
            r->bbox_x = r->x;
            r->bbox_y = r->y;
            r->bbox_w = r->w;
            r->bbox_h = r->h;
            sp = std::move(r);

        } else if (ps.tag == "line") {
            auto l = std::make_unique<Line>();
            l->x1  = attrInt(ps, "x1");           // x1 → x1
            l->y1  = attrInt(ps, "y1");           // y1 → y1
            l->x2  = attrInt(ps, "x2");           // x2 → x2
            l->y2  = attrInt(ps, "y2");           // y2 → y2
            l->bbox_x = std::min(l->x1, l->x2);
            l->bbox_y = std::min(l->y1, l->y2);
            l->bbox_w = std::abs(l->x2 - l->x1);
            l->bbox_h = std::abs(l->y2 - l->y1);
            sp = std::move(l);

        } else {
            continue; // unknown tag
        }

        // Common visual params: fill → fillColour, stroke → strokeColour, stroke-width → strokeWidth
        sp->fillColour   = attrColor(ps, "fill", Qt::black);
        sp->strokeColour = attrColor(ps, "stroke", Qt::transparent);
        sp->strokeWidth  = attrInt(ps, "stroke-width", 0);

        out.push_back(std::move(sp));
    }
    return out;
}
