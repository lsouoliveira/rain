#include <particle_system.h>

ParticleSystem::ParticleSystem() {}

ParticleSystem::ParticleSystem(ParticleSystemOptions options)
    : m_options(options) {}

void ParticleSystem::Init() {
  for (int i = 0; i < m_options.min_particles; i++) {
    SpawnParticle();
  }
}

void ParticleSystem::Draw() {
  BeginShaderMode(m_options.shader);
  for (auto particle : m_particles) {
    DrawRectanglePro(Rectangle{particle->position.x, particle->position.y,
                               particle->size.x, particle->size.y},
                     Vector2{particle->size.x / 2, particle->size.y / 2},
                     particle->rotation, particle->color);
  }
  EndShaderMode();
}

void ParticleSystem::Update(float dt) {
  UpdateParticles(dt);

  if (CanSpawnParticle()) {
    SpawnParticle();
  }
}

void ParticleSystem::UpdateParticles(float dt) {
  for (auto particle : m_particles) {
    particle->position =
        Vector2Add(particle->position, Vector2Scale(particle->velocity, dt));

    if (particle->position.y > m_options.size.y) {
      ResetParticle(particle);
    }
  }
}

void ParticleSystem::Destroy() {
  for (auto particle : m_particles) {
    delete particle;
  }

  m_particles.clear();
}

Particle *ParticleSystem::CreateParticle() { return new Particle(); }

bool ParticleSystem::CanSpawnParticle() {
  return m_particles.size() < m_options.max_particles &&
         RANDOM() < m_options.spawn_rate;
}

void ParticleSystem::SpawnParticle() {
  if (CanSpawnParticle()) {
    m_particles.push_back(ResetParticle(CreateParticle()));
  }
}

Particle *ParticleSystem::ResetParticle(Particle *particle) {
  particle->position = Vector2{RANDOM() * m_options.size.x, 0};
  particle->velocity = m_options.start_velocity;
  particle->rotation = m_options.start_rotation;
  particle->color = m_options.color;
  particle->size = m_options.start_size;

  return particle;
}
