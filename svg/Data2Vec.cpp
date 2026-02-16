#include "Data2Vec.h"

#include <QColor>
#include <QString>

#include "shape_registry.h"

QColor Data2Vec::parseColor(const std::string& name) // QColor is like the standard way to represent colours within Qt.
{
    if (name.empty() || name == "none") return Qt::transparent;
    if (name[0] == '#') return QColor(QString::fromStdString(name));
    
    static const std::unordered_map<std::string, QColor> map = 
    {{"black", Qt::black}, {"white", Qt::white}, {"red", Qt::red}, {"green", Qt::darkGreen}, {"blue", Qt::blue}, {"yellow", Qt::yellow},
    {"cyan", Qt::cyan}, {"magenta", Qt::magenta}, {"gray", Qt::gray},{"orange", QColor(255, 165, 0)}, {"purple", QColor(128, 0, 128)}};

    if (map.count(name)) return map.at(name);
    else return Qt::black;
}

std::vector<std::unique_ptr<Shape>> Data2Vec::convertToShapes(const SvgData& data) { // SvgData has dict of strings, we load them into relelvant object.
  std::vector<std::unique_ptr<Shape>> results;
  const auto& registry = ShapeRegistry::getRegistry(); // const was needed coz we need to promise to not change it, else won't give a reference.
  for (const auto& input_dict : data.elements) {
    for (const auto& trait : registry) { // iterate througth keys. 
      if (input_dict.name != trait.svgTag) continue;
      auto shape = trait.create(); // Initialise a blank shape
      trait.load(shape.get(), input_dict.attributes); // Seperated the loadine logic for specific and generic for simplicity.
      if (input_dict.attributes.count("fill")) shape->fillColour = parseColor(input_dict.attributes.at("fill")); // Handles common attributes
      if (input_dict.attributes.count("stroke")) shape->strokeColour = parseColor(input_dict.attributes.at("stroke"));
      if (input_dict.attributes.count("stroke-width")) shape->strokeWidth = std::stoi(input_dict.attributes.at("stroke-width"));
      results.push_back(std::move(shape)); // Yay, we added to our vector of shapes ready to be drawn on the canvas.
      break;
    }
  }
  return results;
}

