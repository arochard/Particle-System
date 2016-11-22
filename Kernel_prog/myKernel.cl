__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, float2 mouse, float dt)
{
	unsigned int i = get_global_id(0);
	float4 v = vel[i];
	float4 p = pos[i];

	//if (i % 100000 == 0)
	//{
	//	printf("%f\n", p.x);
	//}

}

__kernel void position_begin(__local float4 *p, __local float4 *v, __local float4 *c, __global float4 *pos, __global float4 *color, __global float4 *vel, float pad)
{
	int i = get_global_id(0);
	int	j = get_local_id(0);

	float pi2 = M_PI * 2;
	p = pos[i];
	v = float4(0.0f);
	c = float4(1.0f, 0.0f, 0.0f, 1.0f);

	p.x += sin();
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