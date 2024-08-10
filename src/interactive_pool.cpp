#include "interactive_pool.h"
#include "raylib.h"
#include "raymath.h"
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

  for (int i = 0; i < m_resolution + 1; i++) {
    WavePoint wave_point = {
        Vector2{-transform.size.x / 2 + step * i, -transform.size.y / 2},
        Vector2{0, 0},
        Vector2{-transform.size.x / 2 + step * i, -transform.size.y / 2},
        Vector2{-transform.size.x / 2 + step * i, -transform.size.y / 2}};

    m_wave_points.push_back(wave_point);
  }
}

void InteractivePool::OnDraw() {
  DrawWave();

  if (DEBUG) {
    DrawDebugWavePoints();
  }
}

void InteractivePool::OnUpdate(float dt) {
  m_total_time += dt;

  transform.size.y += m_height_growth_rate * dt;
  transform.position.y -= m_height_growth_rate * dt;

  UpdateWavePoints(dt);
}

void InteractivePool::SetWaterColor(Color color) { m_water_color = color; }

void InteractivePool::SetFoamColor(Color color) { m_foam_color = color; }

std::vector<float> InteractivePool::normalized_foam_color() {
  return NormalizeColor(m_foam_color);
}

std::vector<float> InteractivePool::normalized_water_color() {
  return NormalizeColor(m_water_color);
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

float InteractivePool::SampleYFromRange(float min_x, float max_x) {
  float sample_y;
  int num_points;

  sample_y = 0;
  num_points = 0;

  for (int i = 0; i < m_wave_points.size(); i++) {
    if (GetCenterPoint().x + m_wave_points[i].final_position.x >= min_x &&
        GetCenterPoint().x + m_wave_points[i].final_position.x <= max_x) {
      sample_y += m_wave_points[i].final_position.y;
      num_points++;
    }
  }

  if (num_points == 0) {
    return 0;
  }

  return GetCenterPoint().y + sample_y / num_points;
}

Vector2 InteractivePool::GetClosestPointTo(const Vector2 &point) {
  Vector2 closest_point;
  float min_dist = std::numeric_limits<float>::max();
  float dist;

  for (const WavePoint &wave_point : m_wave_points) {
    dist = Vector2Distance(
        Vector2Add(GetCenterPoint(), wave_point.final_position), point);

    if (dist < min_dist) {
      min_dist = dist;
      closest_point = wave_point.final_position;
    }
  }

  return Vector2{GetCenterPoint().x + closest_point.x,
                 GetCenterPoint().y + closest_point.y};
}

void InteractivePool::UpdateWavePoints(float dt) {
  float force, left_force, right_force;

  for (size_t i = 0; i < m_wave_points.size(); i++) {
    WavePoint &wave_point = m_wave_points[i];

    force = 0;
    force = SPRING_BASELINE_CONSTANT *
            (wave_point.position.y - wave_point.offset.y);

    right_force = left_force = 0;

    if (i == 0) {
      right_force = SPRING_CONSTANT *
                    (m_wave_points[i + 1].offset.y - wave_point.offset.y);
    } else if (i == m_wave_points.size() - 1) {
      left_force = SPRING_CONSTANT *
                   (m_wave_points[i - 1].offset.y - wave_point.offset.y);
    } else {
      right_force = SPRING_CONSTANT *
                    (m_wave_points[i + 1].offset.y - wave_point.offset.y);
      left_force = SPRING_CONSTANT *
                   (m_wave_points[i - 1].offset.y - wave_point.offset.y);
    }

    force += right_force + left_force;

    if (CountPointsUnderInfluence() > MIN_POINTS_UNDER_MOUSE) {
      ApplyWaveInfluenceForce(wave_point, dt);
    }

    wave_point.velocity.y += force * dt;
    wave_point.velocity.y -= wave_point.velocity.y * SPRING_DAMPING_CONSTANT;

    wave_point.offset.y += wave_point.velocity.y * dt;

    wave_point.final_position.y =
        wave_point.offset.y + GetBackgroundWaveHeightAt(wave_point.offset.x);
  }
}

Vector2 InteractivePool::GetCenterPoint() {
  return Vector2{transform.position.x + transform.size.x / 2,
                 transform.position.y + transform.size.y / 2};
}

void InteractivePool::BindShaderValue(const char *name, void *value,
                                      int uniformType) {
  int loc = GetShaderLocation(m_shader, name);
  SetShaderValue(m_shader, loc, value, uniformType);
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

void InteractivePool::DrawPoints(const std::vector<Vector2> &points,
                                 float radius, Color color) {
  for (size_t i = 0; i < points.size(); i++) {
    DrawCircleV(points[i], radius, color);
  }
}

void InteractivePool::DrawDebugWavePoints() {
  float x, y;

  for (const WavePoint &wave_point : m_wave_points) {
    x = transform.position.x + wave_point.final_position.x +
        transform.size.x / 2;
    y = transform.position.y + wave_point.final_position.y +
        transform.size.y / 2;

    DrawCircleV(Vector2{x, y}, 1, GREEN);
  }
}

int InteractivePool::CountPointsUnderInfluence() {
  int count = 0;

  for (const WavePoint &wave_point : m_wave_points) {
    if (IsPointUnderInfluence(wave_point)) {
      count++;
    }
  }

  return count;
}

void InteractivePool::ApplyWaveInfluenceForce(WavePoint &wave_point, float dt) {
  float dist_y;

  if (IsPointUnderInfluence(wave_point)) {
    float dist =
        Vector2Distance(Vector2Add(GetCenterPoint(), wave_point.final_position),
                        GetMousePosition());
    Vector2 direction = Vector2Normalize(
        Vector2Subtract(Vector2Add(GetCenterPoint(), wave_point.final_position),
                        GetMousePosition()));

    if (dist > 0) {
      wave_point.velocity.y += std::max(0.0f, INFLUENCE_RADIUS - dist) /
                               INFLUENCE_RADIUS * INFLUENCE_FORCE * dt;
    }
  }
}

bool InteractivePool::IsPointUnderInfluence(const WavePoint &wave_point) {
  float influence_dist_to_position_y, dist_y;

  influence_dist_to_position_y =
      abs(wave_point.position.y - wave_point.offset.y);

  if (influence_dist_to_position_y > MAX_INFLUENCE_DIST) {
    return 0;
  }

  return CheckCollisionPointCircle(
      Vector2Add(GetCenterPoint(), wave_point.final_position),
      GetMousePosition(), INFLUENCE_RADIUS);
}

WaveRenderData InteractivePool::GenerateWaveRenderData() {
  WaveRenderData wave_data;
  float tex_step;

  tex_step = 1.0f / m_resolution;

  // generate a quad
  wave_data.vertices.push_back(
      Vector2{transform.size.x / 2, transform.size.y / 2});
  wave_data.tex_coords.push_back(Vector2{1, 1});

  for (int i = m_wave_points.size() - 1; i >= 0; i--) {
    wave_data.vertices.push_back(Vector2{m_wave_points[i].final_position.x,
                                         m_wave_points[i].final_position.y});
    wave_data.tex_coords.push_back(Vector2{1 - tex_step * i, 0});
  }

  wave_data.vertices.push_back(
      Vector2{-transform.size.x / 2, transform.size.y / 2});
  wave_data.tex_coords.push_back(Vector2{0, 1});

  return wave_data;
}

}; // namespace Rain