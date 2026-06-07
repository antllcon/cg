#pragma once

#include <chrono>
#include <vector>

class IGpuSorter
{
public:
	virtual ~IGpuSorter() = default;
	virtual std::chrono::nanoseconds Sort(std::vector<int32_t>& data) = 0;
};