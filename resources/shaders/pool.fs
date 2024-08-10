#version 120
#define PI 3.14159265359
#define MAX_WAVES 100

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform vec4 foamColor;
uniform vec4 waterColor;
uniform float foamWidth;
uniform float time;

float wave(
    float A,
    float lambda,
    float w,
    float phi,
    float x,
    float t
) {
    float k; 

    k = (2.0 * PI / lambda);

    return A * sin(k * x - w * t + phi);
}

float sum_waves(
    float parameters[MAX_WAVES * 4],
    int n,
    float x,
    float t
) {
    float y;
    int i;

    y = 0.0;
    for (i = 0; i < n; i++) {
        y += wave(
            parameters[i * 4],
            parameters[i * 4 + 1],
            parameters[i * 4 + 2],
            parameters[i * 4 + 3],
            x,
            t
        ) + parameters[i * 4];
    }

    return y;
}

float waves[MAX_WAVES * 4];

void main() {
  vec2 uv;
  float y;

  waves[0] = 0.01; 
  waves[1] = 0.2; 
  waves[2] = 2.5; 
  waves[3] = 0.0;

  waves[4] = 0.005; 
  waves[5] = 0.1; 
  waves[6] = 4.0; 
  waves[7] = PI / 4.0;

  waves[8] = 0.003; 
  waves[9] = 0.05; 
  waves[10] = 6.0; 
  waves[11] = PI / 2.0;

  uv = fragTexCoord;
  y = sum_waves(waves, 3, uv.x, time);

  if (uv.y >= y && uv.y <= y + foamWidth) {
    gl_FragColor = foamColor;
  } else if (uv.y > y + foamWidth) {
    gl_FragColor = waterColor;
  } else {
    gl_FragColor = vec4(0.0);
  }

  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
