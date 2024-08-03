#ifndef APPLICATION_H
#define APPLICATION_H

#include "core.h"
#include "particle_system.h"
#include "pool.h"
#include <memory>

class Application {
  Color RAIN_COLOR = Color{15, 94, 156, 100};
  Color WATER_COLOR = Color{15, 94, 156, 20};
  Color FOAM_COLOR = Color{15, 94, 156, 40};
  float FOAM_WIDTH = 0.5f;
  float WATER_HEIGHT_GROWTH_RATE = 10.0f;

public:
  Application(int quit_timeout);
  ~Application();

  void Init();
  void Run();

private:
  std::unique_ptr<ParticleSystem> m_rain;
  std::unique_ptr<Pool> m_pool;
  bool m_should_close = false;
  Shader m_rain_shader;
  Shader m_pool_shader;
  Texture m_default_texture;
  int m_quit_timeout;
  float timeout_counter = 0;

  void Update();
  void Draw();
  void Teardown();
  void SetupWindow();
  void SetupWorld();
  ParticleSystem *CreateRainParticleSystem(Shader shader);
  Pool *CreatePool(Shader shader, Texture texture);
};

#endif
