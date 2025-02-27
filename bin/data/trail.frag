// fragment shader

#version 330

out vec4 outputColor;
uniform sampler2DRect particleLayer;
uniform sampler2DRect lastFrame;
uniform sampler2DRect drawLayer;

uniform float center;
uniform float edge;
uniform vec2 size;

void main()
{
  float kernel[9] = float[9](
    edge, edge, edge,
    edge, center, edge,
    edge, edge, edge);


  ivec2 ipos = ivec2(gl_FragCoord.xy);

  vec4 particle = texelFetch(particleLayer, ipos);
  vec4 drawLayer = texelFetch(drawLayer, ivec2(ipos.x, ipos.y));

  vec4 sum = vec4(0, 0, 0, 0);
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      float weight = kernel[(x + 1) * 3 + (y + 1)];
      sum += texelFetch(lastFrame, ipos + ivec2(x, y)) * weight;
    }
  }

  outputColor = clamp(particle + vec4(sum.r, sum.g, sum.b, 0), 0, 1) + vec4(drawLayer.r, drawLayer.g, 0, 0);
}