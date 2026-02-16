#include "base.h"
#include <algorithm>
#include <cmath>

bool Shape::BoolResizeEqually(int handle, double dx, double dy, double& left, double& top, double& right, double& bottom) {
  const double cx = (left + right) / 2.0;
  const double cy = (top + bottom) / 2.0;

  if (handle == Handle::TOP) {
    top += dy;
    const double side = std::max(MIN_SIZE, bottom - top);
    top = bottom - side;
    left = cx - side / 2.0;
    right = cx + side / 2.0;
  } else if (handle == Handle::BOTTOM) {
    bottom += dy;
    const double side = std::max(MIN_SIZE, bottom - top);
    bottom = top + side;
    left = cx - side / 2.0;
    right = cx + side / 2.0;
  } else if (handle == Handle::LEFT) {
    left += dx;
    const double side = std::max(MIN_SIZE, right - left);
    left = right - side;
    top = cy - side / 2.0;
    bottom = cy + side / 2.0;
  } else if (handle == Handle::RIGHT) {
    right += dx;
    const double side = std::max(MIN_SIZE, right - left);
    right = left + side;
    top = cy - side / 2.0;
    bottom = cy + side / 2.0;
  } else {
    return false;
  }
  return true;
}