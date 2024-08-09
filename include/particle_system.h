#pragma once

#include <core.h>
#include <entity.h>
#include <particle.h>
#include <stdlib.h>
#include <vector>

namespace Rain {

struct ParticleSystemOptions {
  Shader shader;
  int min_particles;
  int max_particles;
  Vector2 start_velocity;
  Vector2 start_size;
  float start_rotation;
  float spawn_rate;
  Color color;
};

class ParticleSystem : public Entity {
public:
  ParticleSystem();
  ParticleSystem(ParticleSystemOptions options);

  void Init();
  void OnDraw();
  void OnUpdate(float dt);

private:
  ParticleSystemOptions m_options;
  std::vector<Particle> m_particles;

  Particle CreateParticle();
  void UpdateParticles(float dt);
  bool CanSpawnParticle();
  void SpawnParticle();
  void ResetParticle(Particle &particle);
};

}; // namespace Rain