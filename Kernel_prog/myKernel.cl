__constant float PI2 = M_PI * 2;
__constant float GRAV = 9.81f;
__constant float MASS = 10000;

__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, float2 mouse, float dt)
{
	int i = get_global_id(0);
	float3 mouseCoord = (float3)(mouse.x, mouse.y, 0.0f);
	float3 p = pos[i].xyz;
	float dist = length(p - mouseCoord);
	float force = GRAV * 1000 * 1000 / (dist * dist);

	if ((i % 10000) == 0)
	{
		printf("%f\n", pos[i].x);
	}

	vel[i] += (force / MASS) * dt;
	pos[i] += vel[i];
}

__kernel void position_begin(__global float4 *pos, __global float4 *color, __global float4 *vel, float pad)
{
	int i = get_global_id(0);
	float4 p = pos[i];
	float4 v = vel[i];
	float4 c = color[i];

	//printf("Pad : %f", p.x);

	p = pos[i];
	v = float4(0.0f);
	c = float4(1.0f, 0.0f, 0.0f, 1.0f);

	p.x += (i * pad);
	p.y += (i * pad);
	p.z += (i * pad);
	//p.w = (1 / pad);

	pos[i] = p;
	vel[i] = v;
	color[i] = c;
	if (i % 100000 == 0)
	{
		printf("%f\n", pos[i].x);
	}
}