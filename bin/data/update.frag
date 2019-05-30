// fragment shader

#version 330

uniform sampler2DRect positionData;
uniform sampler2DRect velocityData;
uniform sampler2DRect trailMap;
uniform int numParticles;

uniform float repelThreshold;
uniform float repelStrength;
uniform float attractionThreshold;
uniform float attractionStrength;
uniform float friction;
uniform float speed;
uniform float sensorAngle;
uniform float sensorDistance;
uniform float rotateIncrement;
uniform float time;
uniform vec2 size;

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 velocityOutput;

in vec2 vertTexCoord;

const float PI = 3.1415926535897932384626433832795;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
  ivec2 ipos = ivec2(gl_FragCoord.xy);

  vec4 rawPosition = texelFetch(positionData, ipos);
  vec4 rawVelocity = texelFetch(velocityData, ipos);

  vec2 position = rawPosition.xy * size;
  float heading = rawVelocity.x;

  vec2 velocity = vec2(cos(heading * PI * 2) * speed, sin(heading * PI * 2) * speed);

  // move forward in current direction
  position.x = mod(position.x + velocity.x, size.x);
  position.y = mod(position.y + velocity.y, size.y);

  // read trail map with three sensors
  ivec2 flPos = ivec2(
    position.x + cos(heading * PI * 2 - sensorAngle) * sensorDistance,
    position.y + sin(heading * PI * 2 - sensorAngle) * sensorDistance);

  ivec2 frPos = ivec2(
    position.x + cos(heading * PI * 2 + sensorAngle) * sensorDistance,
    position.y + sin(heading * PI * 2 + sensorAngle) * sensorDistance);

  ivec2 fPos = ivec2(
    position.x + cos(heading * PI * 2) * sensorDistance,
    position.y + sin(heading * PI * 2) * sensorDistance);

  vec4 fl = texelFetch(trailMap, flPos);
  vec4 fr = texelFetch(trailMap, frPos);
  vec4 f = texelFetch(trailMap, fPos);

  // turn based on sensors
  if (f.r > fl.r && f.r > fr.r) {
    // stay on course
  } else if (f.r < fl.r && f.r < fr.r) {
    // rotate randomly
    if (rand(vec2(position.x + time, position.y)) < 0.5) {
      heading += rotateIncrement;
    } else {
      heading -= rotateIncrement;
    }
  } else if (fr.r < fl.r) {
    // turn right
    heading += rotateIncrement;
  } else if (fl.r < fr.r) {
    // turn left
    heading -= rotateIncrement;
  }
  
  positionOutput = vec4(position.x / size.x, position.y / size.y, 0, 1);
  velocityOutput = vec4(heading, 0, 0, 1);
}