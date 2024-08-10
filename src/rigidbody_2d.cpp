#include "rigidbody_2d.h"
#include "entity.h"
#include "raymath.h"

namespace Rain {
Rigidbody2d::Rigidbody2d(Entity *entity) : m_parent(entity) {}

void Rigidbody2d::Init() {}

void Rigidbody2d::OnDraw() {}

void Rigidbody2d::OnUpdate(float dt) {
  m_velocity = Vector2Add(m_velocity, Vector2Scale(m_acceleration, dt));
  m_parent->transform.position =
      Vector2Add(m_parent->transform.position, Vector2Scale(m_velocity, dt));

  m_acceleration = Vector2Zero();
}

void Rigidbody2d::AddForce(Vector2 force) {
  m_acceleration =
      Vector2Add(m_acceleration, Vector2Scale(force, 1.0f / m_mass));
}
}; // namespace Rain