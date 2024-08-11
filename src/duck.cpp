#include "duck.h"

namespace Rain {
Duck::Duck(Texture texture, InteractivePool *interactive_pool)
    : m_texture(texture), m_interactive_pool(interactive_pool) {}

void Duck::Init() {}

void Duck::OnDraw() {
  DrawTexturePro(m_texture, {0, 0, 32, 32},
                 {transform.position.x, transform.position.y, transform.size.x,
                  transform.size.y},
                 {0, 0}, transform.rotation, WHITE);
}

void Duck::OnUpdate(float dt) {
  m_rigidbody->OnUpdate(dt);

  m_rigidbody->AddForce({0, GRAVITY * m_rigidbody->mass()});

  float wave_y = m_interactive_pool->SampleYFromRange(
      transform.position.x, transform.position.x + transform.size.x);

  if (transform.position.y + transform.size.y > wave_y) {
    if (!m_is_underwater) {
      m_is_underwater = true;
      m_rigidbody->SetVelocity({0, 0});
    }

    float submerged_height = std::min(
        transform.size.y, transform.position.y + transform.size.y - wave_y);

    if (submerged_height < 0) {
      submerged_height = 0;
    }

    float submerssion = submerged_height / transform.size.y;
    float fb = submerssion * GRAVITY * 5;

    m_rigidbody->AddForce({0, -fb});
    m_rigidbody->AddForce({0, m_rigidbody->velocity().y * -0.5f});
    m_rigidbody->AddForce({10, 0});
  } else {
    m_is_underwater = false;
  }
}
}; // namespace Rain