// fragment shader

#version 330

uniform sampler2DRect positionData;
uniform sampler2DRect velocityData;
uniform int numParticles;

uniform float repelThreshold;
uniform float repelStrength;
uniform float attractionThreshold;
uniform float attractionStrength;
uniform float friction;

layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 velocityOutput;

in vec2 vertTexCoord;

void main()
{
  // vec4 position = texture(positionData, vertTexCoord);
  ivec2 ipos = ivec2(gl_FragCoord.xy);

  vec4 position = texelFetch(positionData, ipos);
  vec4 rawVelocity = texelFetch(velocityData, ipos);

  // vec4 particle = vec4(0.5, 0.5, 0, 0.5);
  // vec4 particle = texelFetch(backbuffer, ivec2(gl_FragCoord.xy));

  // float vx = (particle.z - 0.5) / 100.;
  // float vy = (particle.w - 0.5) / 100.;
  // particle.x += vx;
  // particle.y += vx;

  vec2 velocity = vec2(rawVelocity.x - 0.5, rawVelocity.y - 0.5);

  if (position.x <= 0) {
    velocity.x *= -1;
  }

  if (position.x >= 1) {
    velocity.x *= -1;
  }

  if (position.y <= 0) {
    velocity.y *= -1;
  }

  if (position.y >= 1) {
    velocity.y *= -1;
  }

  for (int x = 0; x < numParticles; x++) {
    for (int y = 0; y < numParticles; y++) {
      vec4 position2 = texelFetch(positionData, ivec2(x, y));
      if (ipos.x != x && ipos.y != y) {
        vec4 delta = position - position2;
        vec2 r = normalize(delta.xy);
        float dist = length(delta.xy);

        if (dist < repelThreshold) {
          float force = clamp(repelStrength / dist, 0, 0.1);
          velocity += repelStrength * r / dist;
        }

        if (dist < attractionThreshold) {
          float force = clamp(attractionStrength / dist, 0, 0.1);
          velocity -= attractionStrength * r / dist;
        }
      }
    }
  }

  velocity *= friction;
  position.x += velocity.x;
  position.y += velocity.y;

  positionOutput = vec4(position.x, position.y, 0, 1);
  velocityOutput = vec4(velocity.x + 0.5, velocity.y + 0.5, 0, 1);
}