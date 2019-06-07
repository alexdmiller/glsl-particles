// fragment shader

#version 330

uniform sampler2DRect positionData;
uniform sampler2DRect velocityData;
uniform sampler2DRect trailMap;
uniform int numParticles;

uniform float speed;
uniform float sensorAngle;
uniform float sensorDistance;
uniform float rotateIncrement;
uniform float time;
uniform vec2 size;
uniform float waves;

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 velocityOutput;

in vec2 vertTexCoord;

const float PI = 3.1415926535897932384626433832795;

float rand(vec2 co){
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float headingAdjustment(vec2 position, float multiplier, float left, float middle, float right) {
  float heading = 0;
  // turn based on sensors
  if (middle > left && middle > right) {
    // stay on course
  } else if (middle < left && middle < right) {
    // rotate randomly
    if (rand(vec2(position.x + time, position.y)) < 0.5) {
      heading += multiplier * rotateIncrement;
    } else {
      heading -= multiplier * rotateIncrement;
    }
  } else if (right < left) {
    // turn right
    heading += multiplier * rotateIncrement;
  } else if (left < right) {
    // turn left
    heading -= multiplier * rotateIncrement;
  }
  return heading;
}

void main()
{
  ivec2 ipos = ivec2(gl_FragCoord.xy);

  vec4 rawPosition = texelFetch(positionData, ipos);
  vec4 rawVelocity = texelFetch(velocityData, ipos);

  vec2 position = rawPosition.xy * size;
  float heading = rawVelocity.x;
  int team = int(rawPosition.z);

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

  float multiplier1 = mix(-1, 1, team);
  float multiplier2 = mix(1, -1, team);

  heading += headingAdjustment(position, multiplier1, fl.r, f.r, fr.r);
  heading += headingAdjustment(position, multiplier2, fl.g, f.g, fr.g);

  //  position += 1 * vec2(rand(position.xy / 1000) - 0.5, rand(position.xy / 1000 + vec2(100, 100)) - 0.5);
  position.x += sin(position.x / 20 + time * 4) * waves;

  positionOutput = vec4(position.x / size.x, position.y / size.y, rawPosition.z, 1);
  velocityOutput = vec4(heading, 0, 0, 1);
}

