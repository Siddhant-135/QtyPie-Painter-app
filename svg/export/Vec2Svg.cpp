#include "Vec2Svg.h"

#include <QString>
#include <cmath>
#include <fstream>
#include <sstream>

std::string Vec2Svg::ColorToSvg(const QColor& c) {
  if (!c.isValid()) return "none";
  return c.name(QColor::HexRgb).toStdString();
}

std::string Vec2Svg::AttrsCommon(const Shape& s) {
  return " fill=\"" + ColorToSvg(s.fillColour) + "\" stroke=\"" + ColorToSvg(s.strokeColour) + "\" stroke-width=\"" + std::to_string(s.strokeWidth) + "\"";
}

bool Vec2Svg::SaveShapesToSvgFile(const std::vector<std::unique_ptr<Shape>>& shapes, const std::string& filePath, int canvasWidth, int canvasHeight) 
{
  constexpr double kPi = 3.14159265358979323846;
  std::ofstream out(filePath);
  if (!out.is_open()) return false;
  out << "<svg width=\"" << canvasWidth << "\" height=\"" << canvasHeight << "\">\n";
  for (const auto& sp : shapes) {
    const Shape* s = sp.get();
    if (const auto* tb = dynamic_cast<const TextBox*>(s))
    {
      out << "  <textbox x=\"" << tb->x << "\" y=\"" << tb->y
          << "\" width=\"" << tb->w << "\" height=\"" << tb->h
          << "\" font-family=\"" << tb->fontFamily.toStdString()
          << "\" font-size=\"" << tb->fontSize
          << "\" text=\"" << tb->text_line.toStdString()
          << "\"" << AttrsCommon(*tb) << "/>\n";
    }
    else if (const auto* rr = dynamic_cast<const RoundedRectangle*>(s)) 
    {
      out << "  <rect x=\"" << rr->x << "\" y=\"" << rr->y << "\" width=\"" << rr->w  << "\" height=\"" << rr->h << "\" rx=\"" << rr->cornerRadius << "\"" << AttrsCommon(*rr) << "/>\n";
    } 
    else if (const auto* r = dynamic_cast<const Rectangle*>(s)) 
    {
      out << "  <rect x=\"" << r->x << "\" y=\"" << r->y << "\" width=\"" << r->w << "\" height=\"" << r->h << "\"" << AttrsCommon(*r) << "/>\n";
    } 
    else if (const auto* sq = dynamic_cast<const Square*>(s)) 
    {
      out << "  <rect x=\"" << sq->x << "\" y=\"" << sq->y << "\" width=\"" << sq->side << "\" height=\"" << sq->side << "\"" << AttrsCommon(*sq) << "/>\n";
    } 
    else if (const auto* c = dynamic_cast<const Circle*>(s)) 
    {
      out << "  <circle cx=\"" << c->cx << "\" cy=\"" << c->cy << "\" r=\"" << c->radius << "\"" << AttrsCommon(*c) << "/>\n";
    } 
    else if (const auto* l = dynamic_cast<const Line*>(s)) 
    {
      out << "  <line x1=\"" << l->x1 << "\" y1=\"" << l->y1 << "\" x2=\"" << l->x2 << "\" y2=\"" << l->y2 << "\"" << AttrsCommon(*l) << "/>\n";
    }
    else if (const auto* pl = dynamic_cast<const Polyline*>(s))
    {
      std::ostringstream pts;
      for (size_t i = 0; i < pl->worldPts.size(); ++i) {
        if (i > 0) pts << ' ';
        pts << pl->worldPts[i].x() << ',' << pl->worldPts[i].y();
      }
      out << "  <polyline points=\"" << pts.str() << "\"" << AttrsCommon(*pl) << "/>\n";
    }
    else if (const auto* h = dynamic_cast<const Hexagon*>(s))
    {
      std::ostringstream points;
      for (int i = 0; i < 6; ++i) {
        const double ang = kPi + i * (kPi / 3.0);  // start at (cx - R, cy)
        const double x = h->cx + h->R * std::cos(ang);
        const double y = h->cy + h->R * std::sin(ang);
        if (i > 0) points << ' ';
        points << x << ',' << y;
      }
      out << "  <polygon points=\"" << points.str() << "\"" << AttrsCommon(*h) << "/>\n";
    }
  }
  out << "</svg>\n";
  return true;
}
