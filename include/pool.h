#pragma once

#include <core.h>
#include <entity.h>
#include <math.h>
#include <particle.h>

#include <stdlib.h>
#include <vector>

namespace Rain {

struct PoolOptions {
  float height_growth_rate;
  Shader shader;
  Texture texture;
  Color foam_color;
  Color water_color;
  float foam_width;
  float max_height;
};

class Pool : public Entity {
public:
  Pool();
  Pool(PoolOptions options);

  void Init();
  void OnDraw();
  void OnUpdate(float dt);

  void SetWaterColor(Color color);
  void SetFoamColor(Color color);
  void SetFoamWidth(float width);

  void BindShaderValue(const char *name, void *value, int uniformType);

  std::vector<float> normalized_foam_color();
  std::vector<float> normalized_water_color();

private:
  float m_height_growth_rate;
  Shader m_shader;
  Texture m_texture;
  Color m_foam_color;
  Color m_water_color;
  float m_foam_width;
  float m_max_height = 0.0f;
  float m_total_time = 0.0f;

  std::vector<float> NormalizeColor(Color color) {
    return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
            color.a / 255.0f};
  }
};

}; // namespace Rain