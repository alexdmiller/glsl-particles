// vertex shader

#version 330

uniform mat4 modelViewProjectionMatrix;
uniform float time;
uniform vec2 resolution;
uniform sampler2DRect particleData;

in vec4 position;

out vec4 vertColor;

void main(){
  vec4 particle = texelFetch(particleData, ivec2(position.x, position.y));
  
  vec4 transformed = vec4(
    particle.x * resolution.x,
    particle.y * resolution.y,
    0, 1);

  if (particle.z == 0) {
    vertColor = vec4(1, 0, 0, 1);
  } else {
    vertColor = vec4(0, 1, 0, 1);
  }

  gl_Position = modelViewProjectionMatrix * transformed;
}