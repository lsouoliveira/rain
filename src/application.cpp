#include "application.h"
#include <raylib.h>

Application::Application(int quit_timeout) : m_quit_timeout(quit_timeout) {}

Application::~Application() {}

void Application::Init() {
  SetupWindow();
  SetupWorld();
}

void Application::SetupWindow() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_TOPMOST |
                 FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MOUSE_PASSTHROUGH |
                 FLAG_BORDERLESS_WINDOWED_MODE);
  InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Rain");
  SetTargetFPS(60);
}

void Application::SetupWorld() {
  m_rain_shader = LoadShader(0, "resources/shaders/particle.fs");
  m_pool_shader = LoadShader(0, "resources/shaders/pool.fs");
  m_default_texture = LoadTexture("resources/textures/default.png");

  this->m_rain =
      std::unique_ptr<ParticleSystem>(CreateRainParticleSystem(m_rain_shader));
  this->m_rain->Init();

  this->m_pool =
      std::unique_ptr<Pool>(CreatePool(m_pool_shader, m_default_texture));
  this->m_pool->Init();
}

void Application::Run() {
  while (!m_should_close && !WindowShouldClose()) {
    Update();
    Draw();
  }

  Teardown();
}

void Application::Update() {
  timeout_counter += GetFrameTime();

  if (timeout_counter >= m_quit_timeout) { m_should_close = true;
  }

  m_rain->Update();
  m_pool->Update();
}

void Application::Draw() {
  BeginDrawing();

  ClearBackground(CLITERAL(Color){0, 0, 0, 0});
  m_rain->Draw();
  m_pool->Draw();

  EndDrawing();
}

void Application::Teardown() {
  m_rain->Destroy();
  UnloadShader(m_rain_shader);
  UnloadShader(m_pool_shader);
  UnloadTexture(m_default_texture);
  CloseWindow();
}

ParticleSystem *Application::CreateRainParticleSystem(Shader shader) {
  ParticleSystemOptions options{
      .shader = shader,
      .position = Vector2{0, 0},
      .size = Vector2{(float)GetScreenWidth(), (float)GetScreenHeight()},
      .min_particles = 0,
      .max_particles = 1000,
      .start_velocity = Vector2{-250, 1000},
      .start_size = Vector2{1, 40},
      .start_rotation = 10,
      .spawn_rate = 0.5,
      .color = RAIN_COLOR};

  return new ParticleSystem(options);
}

Pool *Application::CreatePool(Shader shader, Texture texture) {
  PoolOptions options{.position = Vector2{0, (float)GetScreenHeight()},
                      .size = Vector2{(float)GetScreenWidth(), 0},
                      .height_growth_rate = WATER_HEIGHT_GROWTH_RATE,
                      .shader = shader,
                      .texture = texture,
                      .foam_color = FOAM_COLOR,
                      .water_color = WATER_COLOR,
                      .foam_width = FOAM_WIDTH,
                      .max_height = (float)GetScreenHeight()};

  return new Pool(options);
}
