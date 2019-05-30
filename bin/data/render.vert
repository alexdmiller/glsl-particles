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

  vertColor = vec4(1, 1, 1, 1);
  gl_Position = modelViewProjectionMatrix * transformed;
}