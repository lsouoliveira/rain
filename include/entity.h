#pragma once

#include <core.h>

namespace Rain {

struct Transform {
  Vector2 position;
  Vector2 size;
  float rotation = 0.0f;
};

class Entity {
public:
  Transform transform;

  virtual void Init() = 0;
  virtual void OnUpdate(float dt) = 0;
  virtual void OnDraw() = 0;
};

}; // namespace Rain