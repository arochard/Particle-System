__constant float RADIUS = 0.5f;
__constant unsigned int	 SPHERE = 1;
__constant unsigned int	 CUBE = 2;


float rand(uint2 *seed)
{
	seed->x = 36969 * (seed->x & 65535) + (seed->x >> 16);
    seed->y = 18000 * (seed->y & 65535) + (seed->y >> 16);
	uint result = (seed->x << 16) + seed->y;
	return (float)result / (float)0xffffffffu;
}


__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float mouse_x, float mouse_y, float mouse_z, float dt, int grav)
{

	int i = get_global_id(0);

	if (i >= n)
		return ;

	float4 p = pos[i];
	float4 mouse = (float4)(mouse_x, mouse_y, 0.0f, 1.0f);
	float4 delta = normalize(mouse - p);
	delta.w = 1.0f;
	float len = fast_length(mouse - p);
	float4 c = color[i];

	if (grav != 0)
	{
		float4 v = vel[i];
		if (len > 0.05f)
		{
			float rdist = mad(delta.x, delta.x, mad(delta.y, delta.y, delta.z * delta.z));
			rdist += 0.1f;

			float inv = native_rsqrt(rdist);
			float invCubed = inv * inv * inv;
			float s = mouse.w * invCubed;

			v += (delta * s) * dt;
			v.w = 0.0f;
		}

		p.x += v.x * dt;
		p.y += v.y * dt;
		p.z += v.z * dt;

		pos[i] = p;
		vel[i] = v;
	}

	c.x = 0.0f + (len / 3);
	c.y = 1.0f - (len / 1.5);
	c.z = 0.0 + (len / 2);
	c.w = 1.0f - (len / 6);

	color[i] = c;
}

__kernel void position_begin(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, unsigned int type, unsigned int seed)
{
	int i = get_global_id(0);

	if (i >= n)
		return ;

	int j = get_local_id(0);
	int k = get_local_size(0);
	float4 p;
	float4 v = float4(0.0f);
	float4 c;

	if (type == SPHERE)
	{
		float deltaTheta = 3.14 / k;
		int ppl = n / k;
		float deltaPhi = (2 * 3.14) / ppl;
		float theta = j * deltaTheta;
		float phi = (i % ppl) * deltaPhi;

		p.x = RADIUS * sin(theta) * cos(phi);
		p.y = RADIUS * sin(theta) * sin(phi);
		p.z = RADIUS * cos(theta);
		p.w = 1.0f;
		p = normalize(p);
	}
	else if (type == CUBE)
	{
		uint2 random = (uint2)(seed * (333 + i), seed / (33 + i));

		p.x = rand(&random) - RADIUS;
		p.y = rand(&random) - RADIUS;
		p.z = rand(&random) - RADIUS;
		p.w = 1.0f;
		p = normalize(p);
	}

	c.x = 0.0f;
	c.y = 1.0f;
	c.z = 0.0f;
	c.w = 1.0f;

	pos[i] = p;
	vel[i] = v;
	color[i] = c;
}