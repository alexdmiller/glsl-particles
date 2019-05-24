// vertex shader

#version 150

uniform mat4 modelViewProjectionMatrix;
uniform float time;
uniform vec2 resolution;
uniform sampler2DRect particleData;

in vec4 position;

void main(){
  vec4 particle = texelFetch(particleData, ivec2(position.x, 0));
  
  vec4 transformed = vec4(
    particle.x * resolution.x,
    particle.y * resolution.y,
    0, 1);

  gl_Position = modelViewProjectionMatrix * transformed;
}