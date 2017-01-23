__constant float RADIUS = 0.5f;
__constant unsigned int	 SPHERE = 1;
__constant unsigned int	 CUBE = 2;

__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float mouse_x, float mouse_y , float dt, int grav)
{

	int i = get_global_id(0);

	if (i >= n || grav == 0)
		return ;

	float4 mouse = (float4)(mouse_x, mouse_y, 0.0f, 1.0f);
	float4 v = vel[i];
	float4 p = pos[i];
	float4 delta = normalize(mouse - p);
	delta.w = 1.0f;
	float len = fast_length(mouse - p);
	float4 c = color[i];

	//DEBUG
	if (i % 100000 == 0)
	{
		//printf("\nMouse x : %f, y = %f, z = %f, w = %f\n", mouse.x, mouse.y, mouse.z, mouse.w);
		//printf("Pos x : %f, y = %f, z = %f, w = %f \n", p.x, p.y, p.z, p.w);
		//printf("Delta x = %f, y = %f, z = %f, w = %f\n", delta.x, delta.y, delta.z, delta.w);
		//printf("len : %f\n", len);
	}


	if (len > 0.05f)
	{
		float rdist = mad(delta.x, delta.x, mad(delta.y, delta.y, delta.z * delta.z));
		rdist += 0.09f;

		float inv = native_rsqrt(rdist);
		float invCubed = inv * inv * inv;
		float s = mouse.w * invCubed;

		v += (delta * s) * dt;
		v.w = 0.0f;
	}

	p.x += v.x * dt;
	p.y += v.y * dt;
	p.z += v.z * dt;

	//c.x = 1.0f - len;
	//c.y = 1.0f - (len / 2);
	//c.z = 1.0f - len;
	c.w = 1.0f;

	color[i] = c;

	pos[i] = p;
	vel[i] = v;
}

__kernel void position_begin(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, unsigned int type)
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

	}

	//DEBUG
	if (p.x > 1.0f)
	{
		printf("x : %f y : %f z : %f\n", p.x, p.y, p.z);
	}

	c.x = 0.0f;
	c.y = 1.0f;
	c.z = 0.0f;
	c.w = 1.0f;

	pos[i] = p;
	vel[i] = v;
	color[i] = c;
}