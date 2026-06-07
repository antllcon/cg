#include "CpuParallelSort.h"
#include <algorithm>
#include <chrono>
#include <execution>

std::chrono::nanoseconds CpuParallelSort::Sort(std::vector<int32_t>& data)
{
	auto start = std::chrono::high_resolution_clock::now();
	std::sort(std::execution::par, data.begin(), data.end());
	return std::chrono::high_resolution_clock::now() - start;
}