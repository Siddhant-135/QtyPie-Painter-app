#pragma once
#include "base.h"
#include <QPointF>
#include <vector>

struct Polyline : public Shape {
  // Centre of the original bounding box
  double cx = 150;
  double cy = 150;

  // Scale factors applied to offsets when reconstructing the drawn points
  double kx = 1.0;
  double ky = 1.0;

  // Each offset stores (dx, dy) relative to (cx, cy) at unit scale.
  // Drawn point = (cx + kx * offset.x(),  cy + ky * offset.y())
  std::vector<QPointF> offsets;

  // Rebuilt every time the shape is moved / resized
  std::vector<QPointF> worldPts;

  /// Re-populate worldPts from offsets + cx,cy + kx,ky
  void rebuild();

  /// Called once after the raw screen points have been collected.
  /// Computes cx, cy, kx=1, ky=1 and converts raw points to offsets.
  void normalise(const std::vector<QPointF>& rawPts);

  void draw_obj(QPainter& p) const override;
  void onBBoxChanged() override;
};
