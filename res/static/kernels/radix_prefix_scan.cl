#define RADIX_SIZE 256

__kernel __attribute__((reqd_work_group_size(RADIX_SIZE, 1, 1)))
void radix_prefix_scan(
	__global uint* histograms,
	const uint numGroups)
{
	__local uint digitTotals[RADIX_SIZE];

	uint digit = get_local_id(0);

	uint myTotal = 0;
	for (uint g = 0; g < numGroups; g++)
		myTotal += histograms[g * RADIX_SIZE + digit];

	digitTotals[digit] = myTotal;
	barrier(CLK_LOCAL_MEM_FENCE);

	uint globalBase = 0;
	for (uint d = 0; d < digit; d++)
		globalBase += digitTotals[d];

	uint cumulative = globalBase;
	for (uint g = 0; g < numGroups; g++)
	{
		uint count = histograms[g * RADIX_SIZE + digit];
		histograms[g * RADIX_SIZE + digit] = cumulative;
		cumulative += count;
	}
}