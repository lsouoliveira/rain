#pragma once

#include "entity.h"
#include "raylib.h"

namespace Rain {
class Rigidbody2d : public Entity {
public:
  Rigidbody2d(Entity *entity);

  void Init();
  void OnUpdate(float dt);
  void OnDraw();

  void AddForce(Vector2 force);

  void SetMass(float mass) { m_mass = mass; }
  float mass() { return m_mass; }

  void SetVelocity(Vector2 velocity) { m_velocity = velocity; }

private:
  Entity *m_parent;

  Vector2 m_velocity = Vector2Zero();
  Vector2 m_acceleration = Vector2Zero();
  Vector2 m_center_of_mass = {0.5, 0.5};

  float m_mass;
  float m_angular_velocity = 0;
  float m_angular_acceleration = 0;
};
}; // namespace Rain