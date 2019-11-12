#version 330

uniform mat4 modelViewProjectionMatrix;
uniform float time;
uniform vec2 resolution;

uniform int instanceGridSize;

uniform sampler2DRect slime;
uniform ivec2 slimeTextureSize;

in vec4 position;
out vec4 vertColor;

void main()
{
	vec2 normalizedInstancePosition = vec2(
		(gl_InstanceID % instanceGridSize) / float(instanceGridSize),
		(gl_InstanceID / instanceGridSize) / float(instanceGridSize));

	vec4 instancePosition = vec4(
		position.x + normalizedInstancePosition.x * slimeTextureSize.x,
		position.y + normalizedInstancePosition.y * slimeTextureSize.y,
		position.z,
		1);

	ivec2 samplePosition = ivec2(normalizedInstancePosition * slimeTextureSize);
	vec4 sample = texture(slime, samplePosition);
	vertColor = sample;

	instancePosition.z = (1 + instancePosition.z) * sample.r * 100.0;

	gl_Position = modelViewProjectionMatrix * instancePosition;
}