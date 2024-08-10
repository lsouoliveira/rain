#version 120

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform vec4 foamColor;
uniform vec4 waterColor;
uniform float foamWidth;

void main() {
  gl_FragColor = waterColor;
}
