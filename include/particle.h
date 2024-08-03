#ifndef PARTICLE_H
#define PARTICLE_H

#include <core.h>

struct Particle {
  Vector2 position;
  Vector2 velocity;
  float rotation;
  Color color;
  Vector2 size;
};

#endif
