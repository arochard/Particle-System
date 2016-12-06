#version 400

// layout(location = 1) in vec3 coord;
layout(location = 0) in vec4 position;
layout(location = 1) in lowp vec4 color;

uniform mat4 MVP;

out vec4 Color;
// flat out int InstanceID;

void main()
{
	//if (position.x == 0.0 && position.y == 0.0)
	//	Color = vec4(1.0, 0.0, 0.0, 1.0);
	Color = color;
	// vec4 eyepos = MVP * position;
	// gl_Position = vec4(eyepos.xy, -eyepos.z, 1.0);
	gl_Position = position;
	gl_PointSize = 1.0;
}