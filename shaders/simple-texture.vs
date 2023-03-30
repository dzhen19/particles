#version 400

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 MVP;
uniform vec3 eyePos;
out vec2 uv;

void main() {
  uv = vUV;

  // vec3 n;
  // n = (eyePos - vPosition) / distance(eyePos, vPosition);
  // float theta;
  // theta = atan(n[2], n[0]);

  // mat3 rot;
  // rot[0] = vec3(cos(theta), 0, sin(theta));
  // rot[1] = vec3(0, 1, 0);
  // rot[2] = vec3(-sin(theta), 0, cos(theta));

  // gl_Position = MVP * vec4(rot * vPosition, 1.0);
  gl_Position = MVP * vec4(vPosition, 1.0);
}
