#version 420 core

layout (location = 0) in vec2 inPos;

uniform ivec2 viewportSize;

void main()
{
	const float pixSize = 0.01;
	gl_Position = vec4(inPos * 2.0 / viewportSize / pixSize, 0, 1);
}
