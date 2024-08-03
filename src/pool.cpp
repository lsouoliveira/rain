#include <pool.h>
#include <raylib.h>

Pool::Pool() {}
Pool::Pool(PoolOptions options)
    : m_position(options.position), m_size(options.size),
      m_height_growth_rate(options.height_growth_rate), m_shader(options.shader),
      m_texture(options.texture), m_foam_color(options.foam_color),
      m_water_color(options.water_color), m_foam_width(options.foam_width),
      m_max_height(options.max_height) {}

void Pool::Init() {
  Vector4 normalized_foam_color = ColorNormalize(m_foam_color);
  int foam_water_loc = GetShaderLocation(m_shader, "foamColor");
  SetShaderValue(m_shader, foam_water_loc,
                 (float[4]){normalized_foam_color.x, normalized_foam_color.y,
                            normalized_foam_color.z, normalized_foam_color.w},
                 SHADER_UNIFORM_VEC4);

  Vector4 normalized_water_color = ColorNormalize(m_water_color);
  int water_color_loc = GetShaderLocation(m_shader, "waterColor");
  SetShaderValue(m_shader, water_color_loc,
                 (float[4]){normalized_water_color.x, normalized_water_color.y,
                            normalized_water_color.z, normalized_water_color.w},
                 SHADER_UNIFORM_VEC4);

  int foam_width_loc = GetShaderLocation(m_shader, "foamWidth");
  SetShaderValue(m_shader, foam_width_loc, &m_foam_width, SHADER_UNIFORM_FLOAT);
}

void Pool::Draw() {
  BeginShaderMode(m_shader);
  DrawTexturePro(m_texture,
                 {0.0, 0.0, (float)m_texture.width, (float)m_texture.height},
                 {m_position.x, m_position.y, m_size.x, m_size.y}, {0.0, 0.0},
                 0.0f, WHITE);
  EndShaderMode();
}

void Pool::Update() {
  m_total_time += GetFrameTime();
  int timeLoc = GetShaderLocation(m_shader, "time");
  SetShaderValue(m_shader, timeLoc, &m_total_time, SHADER_UNIFORM_FLOAT);

  m_size.y += m_height_growth_rate * GetFrameTime();
  m_position.y = (float)GetScreenHeight() - m_size.y;
}

void Pool::Destroy() {}
