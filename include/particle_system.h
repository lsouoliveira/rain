#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <core.h>
#include <particle.h>
#include <stdlib.h>
#include <vector>

struct ParticleSystemOptions {
  Shader shader;
  Vector2 position;
  Vector2 size;
  int min_particles;
  int max_particles;
  Vector2 start_velocity;
  Vector2 start_size;
  float start_rotation;
  float spawn_rate;
  Color color;
};

class ParticleSystem {
public:
  ParticleSystem();
  ParticleSystem(ParticleSystemOptions options);

  void Init();
  void Draw();
  void Update();
  void Destroy();

private:
  ParticleSystemOptions m_options;
  std::vector<Particle *> m_particles;

  Particle *CreateParticle();
  void UpdateParticles();
  bool CanSpawnParticle();
  void SpawnParticle();
  Particle *ResetParticle(Particle *particle);
};

#endif
