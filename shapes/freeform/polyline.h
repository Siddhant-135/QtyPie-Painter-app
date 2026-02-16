#pragma once
#include "../base/shape.h"
#include <QPointF>
#include <vector>

class Polyline : public Shape {
 public:
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
  void Rebuild();

  /// Called once after the raw screen points have been collected.
  /// Computes cx, cy, kx=1, ky=1 and converts raw points to offsets.
  void Normalise(const std::vector<QPointF>& rawPts);

  void DrawObj(QPainter& p) const override;
  void OnBBoxChanged() override;
};
