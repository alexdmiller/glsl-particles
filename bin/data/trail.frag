// fragment shader

#version 330

out vec4 outputColor;
uniform sampler2DRect particleLayer;
uniform sampler2DRect lastFrame;

void main()
{
  ivec2 ipos = ivec2(gl_FragCoord.xy);

  vec4 particle = texelFetch(particleLayer, ipos);

  vec4 sum = vec4(0, 0, 0, 0);
  for (int x = -1; x <= 1; x++) {
    for (int y = -1; y <= 1; y++) {
      sum += texelFetch(lastFrame, ipos + ivec2(x, y));
    }
  }

  sum /= 9.;
  sum -= 0.01;


  outputColor = clamp(particle + sum, 0, 1);
}