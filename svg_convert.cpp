#include "svg_convert.h"
#include "shape_registry.h"
#include <QString>
#include <QColor>

QColor parseColor(const std::string& name) {
    if (name.empty() || name == "none") return Qt::transparent;
    if (name[0] == '#') return QColor(QString::fromStdString(name));
    
    static const std::unordered_map<std::string, QColor> map = {
        {"black", Qt::black}, {"white", Qt::white}, {"red", Qt::red},
        {"green", Qt::darkGreen}, {"blue", Qt::blue}, {"yellow", Qt::yellow},
        {"cyan", Qt::cyan}, {"magenta", Qt::magenta}, {"gray", Qt::gray},
        {"orange", QColor(255,165,0)}, {"purple", QColor(128,0,128)}
    };
    return map.count(name) ? map.at(name) : Qt::black;
}

std::vector<std::unique_ptr<Shape>> convertToShapes(const SvgData& data) {
    std::vector<std::unique_ptr<Shape>> results;
    auto& registry = getRegistry();

    for (const auto& el : data.elements) {
        for (const auto& trait : registry) {
            if (el.name == trait.svgTag) {
                auto shape = trait.create();
                trait.load(shape.get(), el.attributes); // Inflate geometry
                
                // Inflate common styles
                if (el.attributes.count("fill")) 
                    shape->fillColour = parseColor(el.attributes.at("fill"));
                if (el.attributes.count("stroke")) 
                    shape->strokeColour = parseColor(el.attributes.at("stroke"));
                if (el.attributes.count("stroke-width")) 
                    shape->strokeWidth = std::stoi(el.attributes.at("stroke-width"));
                
                results.push_back(std::move(shape));
                break;
            }
        }
    }
    return results;
}