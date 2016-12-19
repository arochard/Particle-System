__constant float PI2 = M_PI * 2.0f;
__constant float GRAV = -0.000981f;
__constant float MASS_PART = 50.0f;
__constant float MASS_CENTER = 100.0f;
__constant float RADIUS = 0.5f;
__constant unsigned int	 SPHERE = 1;
__constant unsigned int	 CUBE = 2;

__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float mouse_x, float mouse_y , float dt)
{
	int i = get_global_id(0);

	if (i >= n)
		return ;
	//if (i % 100000 == 0)
		//printf("%f\n", pos[i].x);
	//float4 p = pos[i];

	if (i % 10000 == 0)
	{
		//printf("%f| %f| %f| %f|\n", pos[i].x, pos[i].y, pos[i].z, pos[i].w);
	}

	float4 v = vel[i];
	float3 mouseCoord = (float3)(mouse_x, mouse_y, 0.0f);
	float3 p = pos[i].xyz;
	float dist = length(p - mouseCoord);
	float force = (GRAV * (MASS_CENTER * MASS_PART)) / (dist * dist);

	if ((i % 10000) == 0)
	{
		//printf("%f\n", dist);
	//	printf("%f %f %f\n", color[i].x, color[i].y, color[i].z);
	//	printf("%f %f %f\n", vel[i].x, vel[i].y, vel[i].z);
	}

	//vel[i] += (force / MASS) * dt;
	p = v.xyz * dt;
	v += (force / MASS_PART) * dt;

	pos[i].xyz = p;
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
	}
	else if (type == CUBE)
	{

	}

	c.x = 0.0f;
	c.y = 1.0f;
	c.z = 0.0f;
	c.w = 1.0f;

	pos[i] = p;
	vel[i] = v;
	color[i] = c;

	if (i < 100)
	{
		//printf("%f| %f| %f| %f|   %f\n", p.x, p.y, p.z, p.w, costheta);
	//	printf("%f %f %f %f\n", color[i].x, color[i].y, color[i].z, color[i].w);
		//printf("i : %d  n : %d\n", i, n);
	}

	//printf("%f %f %f\n", pos[i].x, pos[i].y, pos[i].z);
}