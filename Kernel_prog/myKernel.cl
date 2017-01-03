__constant float GRAV = 6.673f;
__constant float RADIUS = 0.5f;
__constant unsigned int	 SPHERE = 1;
__constant unsigned int	 CUBE = 2;
__constant float 		ESP = 2.0f;

__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float mouse_x, float mouse_y , float dt, int grav, __local float4* pblock)
{

	int gti = get_global_id(0);

	if (gti >= n || grav == 0)
		return ;

	/*
	//if (i < 10 == 0)
	//	printf("%f\n", pos[i].x);
	//float4 p = pos[i];

	float4 mouse = (float4)(mouse_x, mouse_y, 0.0f, 1.0f);
	float4 v = vel[i];

	if ((i % 10000) == 0)
	{
		//printf("%f\n", v.x);
	//	printf("%f %f %f\n", color[i].x, color[i].y, color[i].z);
	//	printf("%f %f %f\n", vel[i].x, vel[i].y, vel[i].z);
	}
	
	float4 d = (float4)(0.0f);
	float rdist;
	float4 p = pos[i];
	
	float4 force;

	d.x = mouse_x - p.x;
	d.y = mouse_y - p.y;
	d.z = 0 - p.z;

	rdist = mad(d.x, d.x, mad(d.y, d.y, d.z * d.z));
	rdist += 0.1f;

	float inv = native_rsqrt(rdist);
	float invCubed = inv * inv * inv;
	float s = mouse.w * invCubed;

	force.x = d.x * s;
	force.y = d.y * s;
	force.z = d.z * s;

	v.x += force.x * dt;
	v.y += force.y * dt;
	v.z += force.z * dt;

	v.x *= 1.0f;
	v.y *= 1.0f;
	v.z *= 1.0f;

	p.x += v.x * dt;
	p.y += v.y * dt;
	p.z += v.z * dt;

	pos[i] = p;
	vel[i] = v;
	*/

	//other approch

	const float4 dtl = (float4)(dt,dt,dt,0.0f);

	int ti = get_local_id(0);
	int ng = get_global_size(0);
	int nt = get_local_size(0);
	int nb = ng/nt;
	if (gti == 1)
		printf("%d %d\n", ng, nt);
	float4 p = pos[gti];
	float4 v = vel[gti];
	float4 a = (float4)(0.0f,0.0f,0.0f,0.0f);

	for(int jb=0; jb < nb; jb++)
	{
		pblock[ti] = pos[jb*nt+ti]; // Cache ONE particle position 
		barrier(CLK_LOCAL_MEM_FENCE); // Wait for others in the work-group 
		for(int j=0; j<nt; j++)
		{ // For ALL cached particle positions ... 
			float4 p2 = pblock[j]; // Read a cached particle position 
			float4 d = p2 - p;
			float invr = rsqrt(d.x*d.x + d.y*d.y + d.z*d.z + 0.1);
			float f = p2.w*invr*invr*invr;
			a += f*d; // Accumulate acceleration 
		}
		barrier(CLK_LOCAL_MEM_FENCE); // Wait for others in work-group 
	}

	p += dtl*v + 0.5f*dtl*dtl*a;
	v += dtl*a;

	pos[gti] = p;
	vel[gti] = v;
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

	//DEBUG
	if (i % 100000 == 0)
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