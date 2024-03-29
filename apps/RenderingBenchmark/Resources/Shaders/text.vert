#version 330

layout(location = 0) in vec4 vertex;

out vec2 texCoords;

uniform mat4 mvp;

void main() {
  gl_Position = mvp * vec4(vertex.xy, 0.0, 1.0);
  texCoords = vertex.zw;
}