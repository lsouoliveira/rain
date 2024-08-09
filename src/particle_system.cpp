#include <particle_system.h>

namespace Rain {

ParticleSystem::ParticleSystem() {}

ParticleSystem::ParticleSystem(ParticleSystemOptions options)
    : m_options(options) {}

void ParticleSystem::Init() {
  m_particles.reserve(m_options.max_particles);

  for (int i = 0; i < m_options.min_particles; i++) {
    SpawnParticle();
  }
}

void ParticleSystem::OnDraw() {
  BeginShaderMode(m_options.shader);
  for (auto &particle : m_particles) {
    DrawRectanglePro(Rectangle{particle.position.x, particle.position.y,
                               particle.size.x, particle.size.y},
                     Vector2{particle.size.x / 2, particle.size.y / 2},
                     particle.rotation, particle.color);
  }
  EndShaderMode();
}

void ParticleSystem::OnUpdate(float dt) {
  UpdateParticles(dt);

  if (CanSpawnParticle()) {
    SpawnParticle();
  }
}

void ParticleSystem::UpdateParticles(float dt) {
  for (auto &particle : m_particles) {
    particle.position =
        Vector2Add(particle.position, Vector2Scale(particle.velocity, dt));

    if (particle.position.y > transform.size.y) {
      ResetParticle(particle);
    }
  }
}

Particle ParticleSystem::CreateParticle() { return Particle(); }

bool ParticleSystem::CanSpawnParticle() {
  return m_particles.size() < m_options.max_particles &&
         RANDOM() < m_options.spawn_rate;
}

void ParticleSystem::SpawnParticle() {
  if (CanSpawnParticle()) {
    Particle particle = CreateParticle();

    ResetParticle(particle);

    m_particles.push_back(particle);
  }
}

void ParticleSystem::ResetParticle(Particle &particle) {
  particle.position = Vector2{RANDOM() * transform.size.x, 0};
  particle.velocity = m_options.start_velocity;
  particle.rotation = m_options.start_rotation;
  particle.color = m_options.color;
  particle.size = m_options.start_size;
}

}; // namespace Rain