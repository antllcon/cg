#pragma once

#include "src/gpu/GpuContext.h"
#include <vector>

class SortBenchmark
{
public:
	explicit SortBenchmark(const GpuContext& ctx);

	void Run(const std::vector<int32_t>& data) const;

private:
	const GpuContext& m_ctx;
};