#pragma once

#include "entity.h"
#include "interactive_pool.h"
#include "raylib.h"
#include "rigidbody_2d.h"
#include <cmath>
#include <memory>

namespace Rain {
class Duck : public Entity {
public:
  Duck(Texture texture, InteractivePool *interactive_pool);

  void Init();
  void OnDraw();
  void OnUpdate(float dt);

  void SetRigidbody2d(Rigidbody2d *rigidbody) {
    m_rigidbody = std::unique_ptr<Rigidbody2d>(rigidbody);
  }

  Vector2 GetCenter() {
    return {transform.position.x + transform.size.x / 2,
            transform.position.y + transform.size.y / 2};
  }

private:
  Texture m_texture;
  InteractivePool *m_interactive_pool;
  std::unique_ptr<Rigidbody2d> m_rigidbody;

  bool m_is_underwater = false;
};
}; // namespace Rain