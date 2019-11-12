#version 330

//declare uniforms
uniform sampler2DRect u_texture;
uniform float resolution;
uniform float radius;
uniform vec2 dir;

out vec4 outputColor;

void main() {
  //this will be our RGBA sum
  vec4 sum = vec4(0.0);
    
  //the amount to blur, i.e. how far off center to sample from 
  //1.0 -> blur by one pixel
  //2.0 -> blur by two pixels, etc.
  float blur = radius; 
    
  //the direction of our blur
  //(1.0, 0.0) -> x-axis blur
  //(0.0, 1.0) -> y-axis blur
  float hstep = dir.x;
  float vstep = dir.y;
    
  //apply blurring, using a 9-tap filter with predefined gaussian weights
    
  sum += texture(u_texture, vec2(gl_FragCoord.x - 4.0*blur*hstep, gl_FragCoord.y - 4.0*blur*vstep)) * 0.0162162162;
  sum += texture(u_texture, vec2(gl_FragCoord.x - 3.0*blur*hstep, gl_FragCoord.y - 3.0*blur*vstep)) * 0.0540540541;
  sum += texture(u_texture, vec2(gl_FragCoord.x - 2.0*blur*hstep, gl_FragCoord.y - 2.0*blur*vstep)) * 0.1216216216;
  sum += texture(u_texture, vec2(gl_FragCoord.x - 1.0*blur*hstep, gl_FragCoord.y - 1.0*blur*vstep)) * 0.1945945946;
  
  sum += texture(u_texture, vec2(gl_FragCoord.x, gl_FragCoord.y)) * 0.2270270270;
  
  sum += texture(u_texture, vec2(gl_FragCoord.x + 1.0*blur*hstep, gl_FragCoord.y + 1.0*blur*vstep)) * 0.1945945946;
  sum += texture(u_texture, vec2(gl_FragCoord.x + 2.0*blur*hstep, gl_FragCoord.y + 2.0*blur*vstep)) * 0.1216216216;
  sum += texture(u_texture, vec2(gl_FragCoord.x + 3.0*blur*hstep, gl_FragCoord.y + 3.0*blur*vstep)) * 0.0540540541;
  sum += texture(u_texture, vec2(gl_FragCoord.x + 4.0*blur*hstep, gl_FragCoord.y + 4.0*blur*vstep)) * 0.0162162162;


  //discard alpha for our simple demo, multiply by vertex color and return
  outputColor = sum;
}