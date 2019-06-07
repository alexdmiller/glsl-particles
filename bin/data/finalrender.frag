// fragment shader

#version 330

out vec4 outputColor;
uniform sampler2DRect image;

void main()
{
  ivec2 ipos = ivec2(gl_FragCoord.xy);

  vec4 pixel = texelFetch(image, ipos);
  outputColor = pixel.r * vec4(0.5, 0.7, 1, 1) + pixel.g * vec4(0.5, 0, 0.25, 1);

  // outputColor = texelFetch(image, ipos);
}