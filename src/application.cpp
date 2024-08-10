#include "application.h"
#include "interactive_pool.h"
#include "raylib.h"
#include "raymath.h"
#include "rigidbody_2d.h"

namespace Rain {

Application::Application(int quit_timeout, int min_particles, int max_particles)
    : m_quit_timeout(quit_timeout), m_min_particles(min_particles),
      m_max_particles(max_particles) {}

Application::~Application() {}

void Application::Init() {
  SetupWindow();
  SetupWorld();
}

void Application::SetupWindow() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST |
                 FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Rain");
  SetWindowState(FLAG_WINDOW_UNDECORATED);
  SetTargetFPS(144);
}

void Application::SetupWorld() {
  m_rain_shader = LoadShader(0, "resources/shaders/particle.fs");
  m_pool_shader = LoadShader(0, "resources/shaders/water.fs");
  m_default_texture = LoadTexture("resources/textures/default.png");
  m_duck_texture = LoadTexture("resources/textures/duck.png");
  m_foreground = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

  this->m_rain =
      std::unique_ptr<ParticleSystem>(CreateRainParticleSystem(m_rain_shader));

  this->m_pool =
      std::unique_ptr<Pool>(CreatePool(m_pool_shader, m_default_texture));

  this->m_interactive_pool = std::unique_ptr<InteractivePool>(
      CreateInteractivePool(m_pool_shader, m_default_texture));

  this->m_duck = std::unique_ptr<Duck>(CreateDuck(m_duck_texture));

  this->m_rain->Init();
  this->m_pool->Init();
  this->m_interactive_pool->Init();
  this->m_duck->Init();
}

void Application::Run() {
  while (!m_should_close && !WindowShouldClose()) {
    Update();
    Draw();
  }

  Teardown();
}

void Application::Update() {
  float dt = GetFrameTime();

  timeout_counter += dt;

  if (timeout_counter >= m_quit_timeout) {
    m_should_close = true;
  }

  m_rain->OnUpdate(dt);
  m_interactive_pool->OnUpdate(dt);

  for (auto &duck : m_ducks) {
    duck->OnUpdate(dt);
  }

  if (IsKeyPressed(KEY_SPACE)) {
    Duck *duck = CreateDuck(m_duck_texture);
    duck->transform.position = Vector2Subtract(
        GetMousePosition(), Vector2Scale(duck->transform.size, 0.5));

    m_ducks.push_back(std::unique_ptr<Duck>(duck));
  }
}

void Application::Draw() { DrawForeground(); }

void Application::DrawForeground() {
  BeginDrawing();
  ClearBackground(BLANK);

  BeginTextureMode(m_foreground);
  ClearBackground(BLANK);
  rlDisableColorBlend();

  m_rain->OnDraw();

  m_interactive_pool->SetFoamColor(FOAM_COLOR);
  m_interactive_pool->SetWaterColor(WATER_COLOR);
  m_interactive_pool->OnDraw();

  rlEnableColorBlend();
  EndTextureMode();

  for (auto &duck : m_ducks) {
    duck->OnDraw();
  }

  DrawTextureRec(m_foreground.texture,
                 {0, 0, (float)m_foreground.texture.width,
                  (float)-m_foreground.texture.height},
                 {0, 0}, WHITE);

  EndDrawing();
}

void Application::Teardown() {
  UnloadShader(m_rain_shader);
  UnloadShader(m_pool_shader);
  UnloadTexture(m_default_texture);
  UnloadTexture(m_duck_texture);
  UnloadRenderTexture(m_foreground);
  CloseWindow();
}

ParticleSystem *Application::CreateRainParticleSystem(Shader shader) {
  ParticleSystem *particle_system = nullptr;
  ParticleSystemOptions options{.shader = shader,
                                .min_particles = m_min_particles,
                                .max_particles = m_max_particles,
                                .start_velocity = Vector2{-250, 1000},
                                .start_size = Vector2{1, 40},
                                .start_rotation = 10,
                                .spawn_rate = 0.5,
                                .color = RAIN_COLOR};

  particle_system = new ParticleSystem(options);
  particle_system->transform.position = Vector2{-RAIN_OFFSET, 0};
  particle_system->transform.size =
      Vector2{(float)GetScreenWidth() + RAIN_OFFSET, (float)GetScreenHeight()};

  return particle_system;
}

Pool *Application::CreatePool(Shader shader, Texture texture) {
  Pool *pool = nullptr;
  PoolOptions options{.height_growth_rate = WATER_HEIGHT_GROWTH_RATE,
                      .shader = shader,
                      .texture = texture,
                      .foam_color = FOAM_COLOR,
                      .water_color = WATER_COLOR,
                      .foam_width = FOAM_WIDTH,
                      .max_height = (float)GetScreenHeight()};

  pool = new Pool(options);
  pool->transform.position = Vector2{0, (float)GetScreenHeight()};
  pool->transform.size = Vector2{(float)GetScreenWidth(), 0};

  return pool;
}

InteractivePool *Application::CreateInteractivePool(Shader shader,
                                                    Texture texture) {
  InteractivePool *interactive_pool = nullptr;
  InteractivePoolOptions options{.resolution = 300,
                                 .height_growth_rate = WATER_HEIGHT_GROWTH_RATE,
                                 .shader = shader,
                                 .texture = texture,
                                 .foam_color = FOAM_COLOR,
                                 .water_color = WATER_COLOR,
                                 .foam_width = FOAM_WIDTH,
                                 .max_height = (float)GetScreenHeight()};

  interactive_pool = new InteractivePool(options);
  interactive_pool->transform.position =
      Vector2{-500, (float)GetScreenHeight()};
  interactive_pool->transform.size =
      Vector2{(float)GetScreenWidth() + 1000, 500};

  return interactive_pool;
}

Duck *Application::CreateDuck(Texture texture) {
  Duck *duck = new Duck(texture, m_interactive_pool.get());
  Rigidbody2d *rigidbody = new Rigidbody2d(duck);

  rigidbody->SetMass(1.25);

  duck->SetRigidbody2d(rigidbody);
  duck->transform.position = Vector2{(float)GetScreenWidth() / 2 - 100,
                                     (float)GetScreenHeight() / 2 - 100};
  duck->transform.size = Vector2{200, 200};

  return duck;
}
}; // namespace Rain