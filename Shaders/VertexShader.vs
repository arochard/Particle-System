#version 400

layout(location = 0) in vec3 coord;
layout(location = 1) in vec4 position;
layout(location = 2) in vec4 color;

out vec4 Color;
// flat out int InstanceID;

void main()
{
	Color = color;
	gl_Position = vec4(coord.xyz + position.xyz, 1.0);
	// InstanceID = gl_InstanceID;
}