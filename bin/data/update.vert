#version 330

uniform mat4 modelViewProjectionMatrix;
in vec4 position;
in vec2 texcoord;

out vec2 vertTexCoord;

void main() {
  gl_Position = modelViewProjectionMatrix * position;
  vertTexCoord = texcoord;
}