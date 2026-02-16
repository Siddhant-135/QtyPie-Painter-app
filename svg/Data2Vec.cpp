#include "Data2Vec.h"

#include "Data2Shape.h"

std::vector<std::unique_ptr<Shape>> Data2Vec::ConvertToShapes(const SvgData& data) {
  std::vector<std::unique_ptr<Shape>> results;
  for (const auto& element : data.elements) {
    auto shape = Data2Shape::Convert(element);
    if (shape) results.push_back(std::move(shape));
  }
  return results;
}

