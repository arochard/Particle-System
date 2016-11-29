#version 400

// layout(location = 1) in vec3 coord;
layout(location = 0) in vec4 position;
layout(location = 1) in lowp vec4 color;

uniform mat4 viewMatrix;

out vec4 Color;
// flat out int InstanceID;

void main()
{
	Color = color;
	gl_Position = position;
	gl_PointSize = 1.0;
}