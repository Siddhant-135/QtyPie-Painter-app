#include <algorithm>
#include <cmath>

#include "shape.h"

bool Shape::BoolResizeEqually(int handle, double dx, double dy, double& left,
                              double& top, double& right, double& bottom) {
  const double cx = (left + right) / 2.0;
  const double cy = (top + bottom) / 2.0;

  if (handle == Handle::kTop) {
    top += dy;
    const double side = std::max(kMinSize, bottom - top);
    top = bottom - side;
    left = cx - side / 2.0;
    right = cx + side / 2.0;
  } else if (handle == Handle::kBottom) {
    bottom += dy;
    const double side = std::max(kMinSize, bottom - top);
    bottom = top + side;
    left = cx - side / 2.0;
    right = cx + side / 2.0;
  } else if (handle == Handle::kLeft) {
    left += dx;
    const double side = std::max(kMinSize, right - left);
    left = right - side;
    top = cy - side / 2.0;
    bottom = cy + side / 2.0;
  } else if (handle == Handle::kRight) {
    right += dx;
    const double side = std::max(kMinSize, right - left);
    right = left + side;
    top = cy - side / 2.0;
    bottom = cy + side / 2.0;
  } else {
    return false;
  }
  return true;
}
