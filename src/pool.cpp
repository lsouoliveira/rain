#include "utils.h"
#include <pool.h>

namespace Rain {

Pool::Pool() {}
Pool::Pool(PoolOptions options)
    : m_height_growth_rate(options.height_growth_rate),
      m_shader(options.shader), m_texture(options.texture),
      m_foam_color(options.foam_color), m_water_color(options.water_color),
      m_foam_width(options.foam_width), m_max_height(options.max_height) {}

void Pool::Init() {
  SetWaterColor(m_water_color);
  SetFoamColor(m_foam_color);
  SetFoamWidth(m_foam_width);
}

void Pool::OnDraw() {
  BeginShaderMode(m_shader);

  BindShaderValue("time", &m_total_time, SHADER_UNIFORM_FLOAT);
  BindShaderValue("waterColor", normalized_water_color().data(),
                  SHADER_UNIFORM_VEC4);
  BindShaderValue("foamColor", normalized_foam_color().data(),
                  SHADER_UNIFORM_VEC4);
  BindShaderValue("foamWidth", &m_foam_width, SHADER_UNIFORM_FLOAT);

  DrawTexturePro(m_texture,
                 {0.0, 0.0, (float)m_texture.width, (float)m_texture.height},
                 {transform.position.x, transform.position.y, transform.size.x,
                  transform.size.y},
                 {0.0, 0.0}, 0.0f, WHITE);
  EndShaderMode();
}

void Pool::OnUpdate(float dt) {
  m_total_time += dt;

  transform.size.y += m_height_growth_rate * GetFrameTime();
  transform.position.y = (float)GetScreenHeight() - transform.size.y;
}

void Pool::SetWaterColor(Color color) { m_water_color = color; }

void Pool::SetFoamColor(Color color) { m_foam_color = color; }

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

void Pool::SetFoamWidth(float width) { m_foam_width = width; }

}; // namespace Rain