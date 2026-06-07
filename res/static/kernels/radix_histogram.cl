#define LOCAL_SIZE 256
#define RADIX_SIZE 256

__kernel __attribute__((reqd_work_group_size(LOCAL_SIZE, 1, 1)))
void radix_histogram(
	__global const uint* input,
	__global uint* histograms,
	const uint shift,
	const uint count)
{
	__local uint localHist[RADIX_SIZE];

	uint lid = get_local_id(0);
	uint gid = get_global_id(0);
	uint groupId = get_group_id(0);

	for (uint i = lid; i < RADIX_SIZE; i += LOCAL_SIZE)
		localHist[i] = 0;
	barrier(CLK_LOCAL_MEM_FENCE);

	if (gid < count)
	{
		uint digit = (input[gid] >> shift) & 0xFF;
		atomic_add(&localHist[digit], 1);
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	for (uint i = lid; i < RADIX_SIZE; i += LOCAL_SIZE)
		histograms[groupId * RADIX_SIZE + i] = localHist[i];
}