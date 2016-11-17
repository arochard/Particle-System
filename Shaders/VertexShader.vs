#version 400

layout(location = 0) in vec3 coord;
layout(location = 1) in vec4 position;
layout(location = 2) in vec4 color;

out Color;

void main()
{
	Color = color;
	gl_position = position;
}