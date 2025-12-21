#version 420 core

uniform bool selected;

out vec4 outColor;

void main()
{
	outColor = selected ? vec4(1, 1, 0, 1) : vec4(1, 1, 1, 1);
}
