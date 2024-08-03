varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform vec4 foamColor;
uniform vec4 waterColor;
uniform float foamWidth;
uniform float time;

float amplitude = 2.0;
float wavelength = 10.0;
float k = 2.0 * 3.14159265 / wavelength;

float wave(float x) {
  return amplitude * sin(k * x + time) * -sin(1.5 * time) * sin(x) - amplitude + cos(x) + sin(x);
}

void main() {
  vec2 uv = fragTexCoord * 100.0;

  float y = 1.0 - wave(uv.x);
  vec4 color = vec4(0.0);

  if (uv.y >= y && uv.y <= y + foamWidth) {
    color = foamColor;
  } else if (uv.y > y + foamWidth) {
    color = waterColor;
  }

  gl_FragColor = color;
}
