__constant float PI2 = M_PI * 2;
__constant float GRAV = -0.0000000981f;
__constant float MASS = 2;
__constant float RADIUS = 0.5f;

//float3 		matrix_mult(__global float* matrix, const float4 *vector)
//{
//	float3 result;
//	float4 tmp = (*vector);

//	result.x = matrix[0] * tmp.x + matrix[4] * tmp.y + matrix[8] * tmp.z + matrix[12] * tmp.w;
//	result.y = matrix[1] * tmp.x + matrix[5] * tmp.y + matrix[9] * tmp.z + matrix[13] * tmp.w;
//	result.z = matrix[2] * tmp.x + matrix[6] * tmp.y + matrix[10] * tmp.z + matrix[14] * tmp.w;

//	return (result);
//}

float 		random(int i, ulong randoms, int s)
{
	ulong seed = randoms + i;
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
	uint result = seed >> s;
	float r = ((float)result / (float)UINT_MAX) * (1.0 - (-1.0)) + (-1.0);
	return(r);
}


__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float mouse_x, float mouse_y , float dt)
{
	int i = get_global_id(0);
	//if (i % 100000 == 0)
		//printf("%f\n", pos[i].x);
	float4 p = pos[i];

	//if (i == 100)
	//{
	//	printf("%f| %f| %f| %f|\n", pos[i].x, pos[i].y, pos[i].z, pos[i].w);
	//}

	if (i < n)
	{
		float4 v = vel[i];
		float3 mouseCoord = (float3)(mouse_x, mouse_y, 0.0f);
		float3 p = pos[i].xyz;
		float dist = length(p - mouseCoord);
		float force = GRAV * MASS * MASS / (dist * dist);

		//if ((i % 10000) == 0)
		//{
		//	printf("%f\n", mouse[0]);
		//	printf("%f %f %f\n", color[i].x, color[i].y, color[i].z);
		//	printf("%f %f %f\n", vel[i].x, vel[i].y, vel[i].z);
		//}

		//vel[i] += (force / MASS) * dt;
		
		p += v.xyz * dt + GRAV/2*(dt*dt);
		v += GRAV * dt;

		pos[i].xyz = p;
		vel[i] = v;
	}
}

__kernel void position_begin(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, ulong randoms)
{
	int i = get_global_id(0);

	//int longitude = i / 16;
   	//int latitude = i % 16;

   	//float sign = -2.0f * (longitude % 2) + 1.0f;
  	float phi = i * PI2 / 100;
   	float theta = i * PI2 / 100;

   	//float x1 = 
   	//float x2 = 

	float4 p;
	float4 v = float4(0.0f, 1.0f, 0.0f, 0.0f);
	float4 c;

	c.x = 0.0f;
	c.y = 1.0f;
	c.z = 0.0f;
	c.w = 1.0f;

	//p.x = random(i, randoms, 16);
	//p.y = random(i, randoms, 16);
	//p.z = random(i, randoms, 16);
	//p.w = 1.f;

	p.x = RADIUS * cos(theta) * cos(phi);
	p.y = RADIUS * cos(theta) * sin(phi);
	p.z = RADIUS * sin(theta);
	p.w = 1.0f;

	if (i <= n)
	{
		pos[i] = p;
		vel[i] = v;
		color[i] = c;
	}
	//if (i % 100000 == 0 && i <= n)
	//{
	//	printf("%f| %f| %f| %f|\n", pos[i].x, pos[i].y, pos[i].z, pos[i].w);
	//	printf("%f %f %f %f\n", color[i].x, color[i].y, color[i].z, color[i].w);
		//printf("i : %d  n : %d\n", i, n);
		//printf("%f %f %f\n\n", p.x, p.y, p.z);
	//}

	//printf("%f %f %f\n", pos[i].x, pos[i].y, pos[i].z);
}