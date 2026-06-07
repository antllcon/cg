#pragma once

#include "IGpuSorter.h"

class CpuParallelSort : public IGpuSorter
{
public:
	std::chrono::nanoseconds Sort(std::vector<int32_t>& data) override;
};