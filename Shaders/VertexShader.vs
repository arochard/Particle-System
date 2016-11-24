#version 400

// layout(location = 1) in vec3 coord;
layout(location = 0) in vec4 position;
layout(location = 1) in lowp vec4 color;

out vec4 Color;
// flat out int InstanceID;

void main()
{
	Color = color;

	gl_Position = position * vec4(0.5, -0.5, 0.0, 1.0);
	// gl_Position = vec4(coord.x + position.x, coord.y + position.y, coord.z + position.z, 1.0);
	// InstanceID = gl_InstanceID;
	gl_PointSize = 20.0;
}