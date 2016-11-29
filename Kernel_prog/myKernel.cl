__constant float PI2 = M_PI * 2;
__constant float GRAV = 0.00981f;
__constant float MASS = 1000;

__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float2 mouse, float dt)
{
	int i = get_global_id(0);
	//float4 p = pos[i];

	if (i < n)
	{
		float4 v = vel[i];
		float3 mouseCoord = (float3)(mouse.x, mouse.y, 0.0f);
		float3 p = pos[i].xyz;
		float dist = length(p - mouseCoord);
		float force = GRAV * 1000 * 1000 / (dist * dist);

		//if ((i % 10000) == 0)
		//{
		//	printf("%f\n", mouse[0]);
		//	printf("%f %f %f\n", color[i].x, color[i].y, color[i].z);
		//	printf("%f %f %f\n", vel[i].x, vel[i].y, vel[i].z);
		//}

		vel[i] += (force / MASS) * dt;
		v.z -= GRAV * dt;
		p.z += v.z * dt;

		pos[i].xyz = p;
		vel[i] = v;
	}
}

__kernel void position_begin(__global float4 *pos, __global float4 *color, __global float4 *vel, int n, float pad)
{
	int i = get_global_id(0);
	int longitude = i / 16;
   	int latitude = i % 16;

   	//float sign = -2.0f * (longitude % 2) + 1.0f;
  	float phi = 2.0f * M_PI_F * longitude/16 + pad;
   	float theta = M_PI_F * latitude/16;

	float4 p;
	float4 v = float4(0.0f, 1.0f, 0.0f, 0.0f);
	float4 c = float4(1.0f, 0.0f, 0.0f, 1.0f);

	//p.x += (i * pad);
	//p.y += (i * pad);
	//p.z += (i * pad);
	//p.w = (1 / pad);

	p.x = 0.5 * sin(theta) * cos(phi);
	p.y = 0.5 * sin(phi) * sin(theta);
	p.z = 0.5 * cos(theta);
	p.w = 1.0f;

	if (i < n)
	{
		pos[i] = p;
		vel[i] = v;
		color[i] = c;
		//printf("%f %f %f\n", color[i].x, color[i].y, color[i].z, color[i].w);
	}
	//if (i % 10000 == 0)
	//{
	//}

	//printf("%f %f %f\n", pos[i].x, pos[i].y, pos[i].z);
}