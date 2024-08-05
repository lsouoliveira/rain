#include <pool.h>
#include <raylib.h>

Pool::Pool() {}
Pool::Pool(PoolOptions options)
    : m_position(options.position), m_size(options.size),
      m_height_growth_rate(options.height_growth_rate),
      m_shader(options.shader), m_texture(options.texture),
      m_foam_color(options.foam_color), m_water_color(options.water_color),
      m_foam_width(options.foam_width), m_max_height(options.max_height) {}

void Pool::Init() {
  SetWaterColor(m_water_color);
  SetFoamColor(m_foam_color);
  SetFoamWidth(m_foam_width);
}

void Pool::Draw() {
  BeginShaderMode(m_shader);

  BindShaderValue("time", &m_total_time, SHADER_UNIFORM_FLOAT);
  BindShaderValue("waterColor", normalized_water_color().data(),
                  SHADER_UNIFORM_VEC4);
  BindShaderValue("foamColor", normalized_foam_color().data(),
      SHADER_UNIFORM_VEC4);
  BindShaderValue("foamWidth", &m_foam_width, SHADER_UNIFORM_FLOAT);

  DrawTexturePro(m_texture,
                 {0.0, 0.0, (float)m_texture.width, (float)m_texture.height},
                 {m_position.x, m_position.y, m_size.x, m_size.y}, {0.0, 0.0},
                 0.0f, WHITE);
  EndShaderMode();
}

void Pool::Update(float dt) {
  m_total_time += dt;

  m_size.y += m_height_growth_rate * GetFrameTime();
  m_position.y = (float)GetScreenHeight() - m_size.y;
}

void Pool::Destroy() {}

void Pool::SetWaterColor(Color color) {
  m_water_color = color;
}

void Pool::SetFoamColor(Color color) {
  m_foam_color = color;
}

void Pool::BindShaderValue(const char *name, void *value, int uniformType) {
  int loc = GetShaderLocation(m_shader, name);
  SetShaderValue(m_shader, loc, value, uniformType);
}

std::vector<float> Pool::normalized_foam_color() {
  return NormalizeColor(m_foam_color);
}

std::vector<float> Pool::normalized_water_color() {
  return NormalizeColor(m_water_color);
}

void Pool::SetFoamWidth(float width) {
  m_foam_width = width;
}
