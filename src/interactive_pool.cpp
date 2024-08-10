#include "interactive_pool.h"
#include "raylib.h"
#include "utils.h"

namespace Rain {

InteractivePool::InteractivePool() {}

InteractivePool::InteractivePool(const InteractivePoolOptions &options)
    : m_height_growth_rate(options.height_growth_rate),
      m_resolution(options.resolution), m_shader(options.shader),
      m_texture(options.texture), m_foam_color(options.foam_color),
      m_water_color(options.water_color), m_foam_width(options.foam_width),
      m_max_height(options.max_height) {}

void InteractivePool::Init() {
  float step = transform.size.x / m_resolution;

  for (int i = 0; i <= m_resolution; i++) {
    WavePoint wave_point = {
        Vector2{-transform.size.x / 2 + step * i, -transform.size.y / 2},
        Vector2{0, 0},
        Vector2{-transform.size.x / 2 + step * i, -transform.size.y / 2}};

    m_wave_points.push_back(wave_point);
  }
}

void InteractivePool::OnUpdate(float dt) {
  m_total_time += dt;

  UpdateWavePoints(dt);
}

void InteractivePool::UpdateWavePoints(float dt) {
  for (size_t i = 0; i < m_wave_points.size(); i++) {
    WavePoint &wave_point = m_wave_points[i];

    wave_point.offset.y =
        wave_point.position.y + GetBackgroundWaveHeightAt(wave_point.offset.x);
  }
}

void InteractivePool::DrawWave() {
  WaveRenderData wave_data = GenerateWaveRenderData();

  BeginShaderMode(m_shader);

  BindShaderValue("waterColor", normalized_water_color().data(),
                  SHADER_UNIFORM_VEC4);
  BindShaderValue("foamColor", normalized_foam_color().data(),
                  SHADER_UNIFORM_VEC4);
  BindShaderValue("foamWidth", &m_foam_width, SHADER_UNIFORM_FLOAT);

  DrawPolygon(m_texture,
              Vector2{transform.position.x + transform.size.x / 2,
                      transform.position.y + transform.size.y / 2},
              wave_data.vertices.data(), wave_data.tex_coords.data(),
              wave_data.vertices.size(), WHITE);

  EndShaderMode();
}

void InteractivePool::DrawDebugWavePoints() {
  float x, y;

  for (const WavePoint &wave_point : m_wave_points) {
    x = transform.position.x + wave_point.offset.x + transform.size.x / 2;
    y = transform.position.y + wave_point.offset.y + transform.size.y / 2;

    DrawCircleV(Vector2{x, y}, 10, GREEN);
  }
}

WaveRenderData InteractivePool::GenerateWaveRenderData() {
  WaveRenderData wave_data;
  float tex_step;

  tex_step = 1.0f / m_resolution;

  wave_data.vertices.push_back(
      Vector2{-transform.size.x / 2, transform.size.y / 2});
  wave_data.tex_coords.push_back(Vector2{0, 1});

  wave_data.vertices.push_back(
      Vector2{transform.size.x / 2, transform.size.y / 2});
  wave_data.tex_coords.push_back(Vector2{1, 1});

  for (int i = m_resolution; i >= 0; i--) {
    WavePoint wave_point = m_wave_points[i];

    wave_data.vertices.push_back(wave_point.offset);
    wave_data.tex_coords.push_back(Vector2{1 - tex_step * i, 0});
  }

  wave_data.vertices.push_back(
      Vector2{-transform.size.x / 2, transform.size.y / 2});
  wave_data.tex_coords.push_back(Vector2{0, 1});

  return wave_data;
}

void InteractivePool::OnDraw() {
  DrawWave();
  DrawDebugWavePoints();
}

void InteractivePool::SetWaterColor(Color color) { m_water_color = color; }

void InteractivePool::SetFoamColor(Color color) { m_foam_color = color; }

std::vector<float> InteractivePool::normalized_foam_color() {
  return NormalizeColor(m_foam_color);
}

std::vector<float> InteractivePool::normalized_water_color() {
  return NormalizeColor(m_water_color);
}

void InteractivePool::BindShaderValue(const char *name, void *value,
                                      int uniformType) {
  int loc = GetShaderLocation(m_shader, name);
  SetShaderValue(m_shader, loc, value, uniformType);
}

float InteractivePool::GetBackgroundWaveHeightAt(const float &x) {
  float y;

  y = 0;

  for (size_t i = 0; i < MAX_BACKGROUND_WAVES; i++) {
    y += ComputeWave(x, m_total_time, m_waves_parameters[i][0],
                     m_waves_parameters[i][1], m_waves_parameters[i][2],
                     m_waves_parameters[i][3]);
  }

  return y;
}

float InteractivePool::ComputeWave(float x, float t, float amplitude,
                                   float wave_length, float frequency,
                                   float phase) {
  float k;

  k = 2 * PI / wave_length;

  return amplitude * sin(k * x - frequency * t + phase) + amplitude;
}

}; // namespace Rain