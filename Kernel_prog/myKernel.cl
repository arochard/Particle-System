__constant float PI2 = M_PI * 2;
__constant float GRAV = -0.0000000981f;
//__constant float MASS = 2;
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


__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float mouse_x, float mouse_y , float dt)
{
	int i = get_global_id(0);

	if (i >= n)
		return ;
	//if (i % 100000 == 0)
		//printf("%f\n", pos[i].x);
	//float4 p = pos[i];

	//if (i == 100)
	//{
	//	printf("%f| %f| %f| %f|\n", pos[i].x, pos[i].y, pos[i].z, pos[i].w);
	//}

	float4 v = vel[i];
	float3 mouseCoord = (float3)(mouse_x, mouse_y, 0.0f);
	float3 p = pos[i].xyz;
	float dist = length(p - mouseCoord);
	//float force = GRAV * MASS * MASS / (dist * dist);

	if ((i % 10000) == 0)
	{
		printf("%f\n", dist);
	//	printf("%f %f %f\n", color[i].x, color[i].y, color[i].z);
	//	printf("%f %f %f\n", vel[i].x, vel[i].y, vel[i].z);
	}

	//vel[i] += (force / MASS) * dt;
	
	p += v.xyz * dt + GRAV/2*(dt*dt);
	v = GRAV * dt;

	pos[i].xyz = p;
	vel[i] = v;
}

__kernel void position_begin(__global float4 *pos, __global float4 *color, __global float4 *vel, int n)
{
	int i = get_global_id(0);

	if (i >= n)
		return ;

	int rings = n / 180;
	int pointRing = n / rings;

	float phi_step = M_PI_2 / n;
	float costheta_step = 2 / n;


	//int longitude = i / 16;
   	//int latitude = i % 16;

   	//float sign = -2.0f * (longitude % 2) + 1.0f;
  	float phi = 0 + (phi_step * i);
  	float costheta = -1 + (costheta_step * i);
  	//float u = random(i, randoms, 0, 1);
   	float theta = acos(costheta);
   	//float r = RADIUS * sqrt(u);

   	//float stp = 1 / n;

   	//float x1 = 
   	//float x2 = 

	float4 p;
	float4 v = float4(0.0f);
	float4 c;

	c.x = 0.0f;
	c.y = 1.0f;
	c.z = 0.0f;
	c.w = 1.0f;

//	p.x = random(i, randX, -0.7, 0.7);
//	p.y = random(i, randY, -0.7, 0.7);
//	p.z = random(i, randZ, -0.7, 0.7);
//	p.w = 1.f;

	p.x = RADIUS * sin(theta) * cos(phi);
	p.y = RADIUS * sin(theta) * sin(phi);
	p.z = RADIUS * cos(theta);
	p.w = 1.0f;

	//p.x = 0.0;
	//p.y = 0.0;
	//p.z = 0.0;
	//p.w = 1.0;

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