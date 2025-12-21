#version 420 core

layout (location = 0) in vec2 inPosQuad;

out float angle1Rad;
out float angle2Rad;

void main()
{
	const float pi = 3.1415927;
	angle1Rad = inPosQuad.x * pi;
	angle2Rad = inPosQuad.y * pi;
	gl_Position = vec4(inPosQuad, 0, 1);
}
