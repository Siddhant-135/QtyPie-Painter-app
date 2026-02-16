#include "Data2Shape.h"

#include <QColor>
#include <QString>

#include "shape_registry.h"

namespace {
QColor parseColor(const std::string& name) {
  if (name.empty() || name == "none") return Qt::transparent;
  if (name[0] == '#') return QColor(QString::fromStdString(name));

  static const std::unordered_map<std::string, QColor> map =
    {{"black", Qt::black}, {"white", Qt::white}, {"red", Qt::red}, {"green", Qt::darkGreen}, {"blue", Qt::blue}, {"yellow", Qt::yellow},
     {"cyan", Qt::cyan}, {"magenta", Qt::magenta}, {"gray", Qt::gray}, {"orange", QColor(255, 165, 0)}, {"purple", QColor(128, 0, 128)}};

  if (map.count(name)) return map.at(name);
  return Qt::black;
}
}

std::unique_ptr<Shape> Data2Shape::convert(const SvgTag& tag) {
  const auto& registry = ShapeRegistry::getRegistry();
  for (const auto& trait : registry) {
    if (tag.name != trait.svgTag) continue;
    auto shape = trait.create();
    trait.load(shape.get(), tag.attributes);
    if (tag.attributes.count("fill")) shape->fillColour = parseColor(tag.attributes.at("fill"));
    if (tag.attributes.count("stroke")) shape->strokeColour = parseColor(tag.attributes.at("stroke"));
    if (tag.attributes.count("stroke-width")) shape->strokeWidth = std::stoi(tag.attributes.at("stroke-width"));
    return shape;
  }
  return nullptr;
}
