#include "Vec2Svg.h"

#include <QString>
#include <fstream>

std::string Vec2Svg::colorToSvg(const QColor& c) {
  if (!c.isValid()) return "none";
  return c.name(QColor::HexRgb).toStdString();
}

std::string Vec2Svg::attrsCommon(const Shape& s) {
  return " fill=\"" + colorToSvg(s.fillColour) + "\" stroke=\"" + colorToSvg(s.strokeColour) + "\" stroke-width=\"" + std::to_string(s.strokeWidth) + "\"";
}

bool Vec2Svg::saveShapesToSvgFile(const std::vector<std::unique_ptr<Shape>>& shapes, const std::string& filePath, int canvasWidth, int canvasHeight) 
{
  std::ofstream out(filePath);
  if (!out.is_open()) return false;
  out << "<svg width=\"" << canvasWidth << "\" height=\"" << canvasHeight << "\n";
  for (const auto& sp : shapes) {
    const Shape* s = sp.get();
    if (const auto* rr = dynamic_cast<const RoundedRectangle*>(s)) 
    {
      out << "  <rect x=\"" << rr->x << "\" y=\"" << rr->y << "\" width=\"" << rr->w  << "\" height=\"" << rr->h << "\" rx=\"" << rr->cornerRadius << "\"" << attrsCommon(*rr) << "/>\n";
    } 
    else if (const auto* r = dynamic_cast<const Rectangle*>(s)) 
    {
      out << "  <rect x=\"" << r->x << "\" y=\"" << r->y << "\" width=\"" << r->w << "\" height=\"" << r->h << "\"" << attrsCommon(*r) << "/>\n";
    } 
    else if (const auto* sq = dynamic_cast<const Square*>(s)) 
    {
      out << "  <rect x=\"" << sq->x << "\" y=\"" << sq->y << "\" width=\"" << sq->side << "\" height=\"" << sq->side << "\"" << attrsCommon(*sq) << "/>\n";
    } 
    else if (const auto* c = dynamic_cast<const Circle*>(s)) 
    {
      out << "  <circle cx=\"" << c->cx << "\" cy=\"" << c->cy << "\" r=\"" << c->radius << "\"" << attrsCommon(*c) << "/>\n";
    } 
    else if (const auto* l = dynamic_cast<const Line*>(s)) 
    {
      out << "  <line x1=\"" << l->x1 << "\" y1=\"" << l->y1 << "\" x2=\"" << l->x2 << "\" y2=\"" << l->y2 << "\"" << attrsCommon(*l) << "/>\n";
    }
  }
  out << "</svg>\n";
  return true;
}