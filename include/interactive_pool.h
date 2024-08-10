#pragma once

#include "renderer.h"
#include <cmath>
#include <core.h>
#include <entity.h>
#include <particle.h>
#include <stdlib.h>
#include <vector>

namespace Rain {

struct InteractivePoolOptions {
  int resolution;
  float height_growth_rate;
  Shader shader;
  Texture texture;
  Color foam_color;
  Color water_color;
  float foam_width;
  float max_height;
};

struct WavePoint {
  Vector2 position;
  Vector2 velocity;
  Vector2 offset;
  Vector2 final_position;
};

struct WaveRenderData {
  std::vector<Vector2> vertices;
  std::vector<Vector2> tex_coords;
};

class InteractivePool : public Entity {
public:
  const static int MAX_BACKGROUND_WAVES = 3;
  const static int MIN_POINTS_UNDER_MOUSE = 0;
  constexpr static float INFLUENCE_RADIUS = 200.0;
  constexpr static float MAX_INFLUENCE_DIST = 300.0;
  constexpr static float SPRING_CONSTANT = 4;
  constexpr static float SPRING_BASELINE_CONSTANT = 2;
  constexpr static float SPRING_DAMPING_CONSTANT = 0.001;
  constexpr static float INFLUENCE_FORCE = 80;
  constexpr static float TIME_SCALE = 1;

  InteractivePool();
  InteractivePool(const InteractivePoolOptions &options);

  void Init();
  void OnDraw();
  void OnUpdate(float dt);

  void SetWaterColor(Color color);
  void SetFoamColor(Color color);

  std::vector<float> normalized_foam_color();
  std::vector<float> normalized_water_color();

  float GetBackgroundWaveHeightAt(const float &x);
  float ComputeWave(float x, float t, float amplitude, float wave_length,
                    float frequency, float phase);

  void UpdateWavePoints(float dt);

  Vector2 GetCenterPoint();

  void BindShaderValue(const char *name, void *value, int uniformType);

private:
  int m_resolution;
  float m_height_growth_rate;
  Shader m_shader;
  Texture m_texture;
  Color m_foam_color;
  Color m_water_color;
  float m_foam_width;
  float m_max_height;
  float m_total_time = 0;
  std::vector<WavePoint> m_wave_points;
  float m_waves_parameters[MAX_BACKGROUND_WAVES][4] = {
      {5, 400, 1, 10.0}, {4, 500, 1, 10.0}, {2, 600, 1, 10.0}};

  void DrawWave();
  void DrawDebugWavePoints();
  void DrawPoints(const std::vector<Vector2> &points, float radius,
                  Color color);

  int CountPointsUnderInfluence();

  bool IsPointUnderInfluence(const WavePoint &wave_point);

  void ApplyWaveInfluenceForce(WavePoint &wave_point, float dt);

  WaveRenderData GenerateWaveRenderData();
};
}; // namespace Rain