#version 330

uniform sampler2DRect layer1;
uniform sampler2DRect layer2;
uniform float level1;
uniform float level2;

out vec4 outputColor;

void main()
{
  ivec2 ipos = ivec2(gl_FragCoord.xy);
  outputColor = clamp(texelFetch(layer1, ipos) * level1 + texelFetch(layer2, ipos) * level2, 0, 1);
//  outputColor = vec4(1.0,0.0,0.0,1.0);
}