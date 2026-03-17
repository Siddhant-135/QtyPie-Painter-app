#include "polyline.h"

#include <algorithm>
#include <cmath>
#include <limits>

void Polyline::Normalise(const std::vector<QPointF>& rawPts) {
  if (rawPts.empty()) return;

  double minX = std::numeric_limits<double>::max();
  double minY = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::lowest();
  double maxY = std::numeric_limits<double>::lowest();

  for (const auto& p : rawPts) {
    minX = std::min(minX, p.x());
    minY = std::min(minY, p.y());
    maxX = std::max(maxX, p.x());
    maxY = std::max(maxY, p.y());
  }

  cx = (minX + maxX) / 2.0;
  cy = (minY + maxY) / 2.0;
  kx = 1.0;
  ky = 1.0;

  // Store offsets relative to centre
  offsets.clear();
  offsets.reserve(rawPts.size());
  for (const auto& p : rawPts) offsets.emplace_back(p.x() - cx, p.y() - cy);

  // Set bounding box
  double bw = maxX - minX;
  double bh = maxY - minY;
  if (bw < kMinSize) bw = kMinSize;
  if (bh < kMinSize) bh = kMinSize;
  bbox_x = cx - bw / 2.0;
  bbox_y = cy - bh / 2.0;
  bbox_w = bw;
  bbox_h = bh;

  Rebuild();
}

void Polyline::Rebuild() {
  worldPts.resize(offsets.size());
  for (size_t i = 0; i < offsets.size(); ++i)
    worldPts[i] = QPointF(cx + kx * offsets[i].x(), cy + ky * offsets[i].y());
}

void Polyline::DrawObj(QPainter& p) const {
  if (worldPts.size() < 2) return;
  p.setPen(QPen(strokeColour, strokeWidth));
  p.setBrush(fillColour);
  QPolygonF poly;
  poly.reserve(static_cast<int>(worldPts.size()));
  for (const auto& pt : worldPts) poly << pt;
  p.drawPolyline(poly);
}

void Polyline::OnBBoxChanged() {
  // Compute the half-extents of the original offset cloud
  double halfW = bbox_w / 2.0;
  double halfH = bbox_h / 2.0;

  // New centre
  cx = bbox_x + halfW;
  cy = bbox_y + halfH;

  // Compute original half-extent from offsets
  double origHalfW = 0;
  double origHalfH = 0;
  for (const auto& o : offsets) {
    origHalfW = std::max(origHalfW, std::abs(o.x()));
    origHalfH = std::max(origHalfH, std::abs(o.y()));
  }

  kx = (origHalfW > 1e-9) ? (halfW / origHalfW) : 1.0;
  ky = (origHalfH > 1e-9) ? (halfH / origHalfH) : 1.0;

  Rebuild();
}
