#define LOCAL_SIZE 256
#define RADIX_SIZE 256

__kernel __attribute__((reqd_work_group_size(LOCAL_SIZE, 1, 1)))
void radix_scatter(
	__global const uint* input,
	__global uint* output,
	__global const uint* offsets,
	const uint shift,
	const uint count)
{
	__local uint localDigits[LOCAL_SIZE];

	uint lid = get_local_id(0);
	uint gid = get_global_id(0);
	uint groupId = get_group_id(0);

	uint val = (gid < count) ? input[gid] : 0xFFFFFFFF;
	uint digit = (val >> shift) & 0xFF;

	localDigits[lid] = (gid < count) ? digit : 0xFFFFFFFF;
	barrier(CLK_LOCAL_MEM_FENCE);

	if (gid < count)
	{
		uint groupStart = groupId * LOCAL_SIZE;
		uint localRank = 0;
		for (uint i = 0; i < lid; i++)
		{
			if ((groupStart + i) < count && localDigits[i] == digit)
				localRank++;
		}

		uint pos = offsets[groupId * RADIX_SIZE + digit] + localRank;
		output[pos] = val;
	}
}