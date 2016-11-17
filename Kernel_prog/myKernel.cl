__kernel void update_position(__global float4 *pos, __global float4 *color, __global float4 *vel, __global float2 *mouse)
{
	unsigned int i = get_global_id(0);
	float4 v = vel[i];
	float4 p = pos[i];


}

__kernel void position_begin(__global float4 *pos, __global float4 *vel, __global const float *pad)
{
	unsigned int i = get_global_id(0);

	float4 p = pos[i];
	float4 v = float4(0.0f);


	p.x += i * pad;
	p.y += i * pad;

	pos[i] = p;
	vel[i] = v;
}