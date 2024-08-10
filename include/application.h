#pragma once

#include "core.h"
#include "duck.h"
#include "interactive_pool.h"
#include "particle_system.h"
#include "pool.h"
#include "raylib.h"

#include <memory>

namespace Rain {

class Application {
  Color RAIN_COLOR = Color{15, 94, 156, 200};
  Color WATER_COLOR = Color{15, 94, 156, 200};
  Color FOAM_COLOR = Color{5, 48, 82, 60};
  float FOAM_WIDTH = 0.01f;
  float WATER_HEIGHT_GROWTH_RATE = 25.0f;
  float RAIN_OFFSET = 500.0f;

public:
  Application(int quit_timeout, int min_particles, int max_particles);
  ~Application();

  void Init();
  void Run();

private:
  std::unique_ptr<ParticleSystem> m_rain;
  std::unique_ptr<Pool> m_pool;
  std::unique_ptr<InteractivePool> m_interactive_pool;
  std::unique_ptr<Duck> m_duck;
  std::vector<std::unique_ptr<Duck>> m_ducks;

  Shader m_rain_shader;
  Shader m_pool_shader;

  Texture m_default_texture;
  Texture m_duck_texture;

  RenderTexture m_foreground;

  int m_quit_timeout;
  int m_min_particles;
  int m_max_particles;

  float timeout_counter = 0;

  bool m_should_close = false;

  void Update();
  void Draw();
  void DrawBackground();
  void DrawForeground();
  void Teardown();
  void SetupWindow();
  void SetupWorld();

  ParticleSystem *CreateRainParticleSystem(Shader shader);
  Pool *CreatePool(Shader shader, Texture texture);
  InteractivePool *CreateInteractivePool(Shader shader, Texture texture);
  Duck *CreateDuck(Texture texture);
};

}; // namespace Rain