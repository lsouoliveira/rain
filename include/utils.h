#pragma once

#include <raylib.h>
#include <vector>

namespace Rain {
static std::vector<float> NormalizeColor(Color color) {
  return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
          color.a / 255.0f};
}
}; // namespace Rain