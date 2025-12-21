#version 420 core

layout (location = 0) in vec2 inPosQuad;

uniform vec2 pos;
uniform vec2 size;
uniform ivec2 viewportSize;

void main()
{
	const float pixSize = 0.01;
	vec2 posWorld = pos + inPosQuad * size / 2.0;
	gl_Position = vec4(posWorld * 2.0 / viewportSize / pixSize, 0, 1);
}
