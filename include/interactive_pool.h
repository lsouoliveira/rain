#pragma once

#include <core.h>
#include <entity.h>
#include <math.h>
#include <particle.h>
#include <stdlib.h>
#include <vector>

namespace Rain {

struct InteractivePoolOptions {
  Vector2 position;
  int resolution;
};

class InteractivePool : public Entity {
public:
  InteractivePool();
  InteractivePool(const InteractivePoolOptions &options);

  void Init();
  void OnDraw();
  void OnUpdate(float dt);

private:
  int m_resolution;
};

}; // namespace Rain