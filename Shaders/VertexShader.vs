#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in lowp vec4 color;

uniform mat4 MVP;

out vec4 Color;

void 		main()
{
	gl_Position = MVP * position;
	Color = color;
	gl_PointSize = 1.0;
}