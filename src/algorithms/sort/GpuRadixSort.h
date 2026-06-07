#pragma once

#include "IGpuSorter.h"
#include "gpu/GpuContext.h"
#include "gpu/GpuKernel.h"
#include "gpu/GpuProgram.h"

class GpuRadixSort : public IGpuSorter
{
public:
	explicit GpuRadixSort(const GpuContext& ctx);

	std::chrono::nanoseconds Sort(std::vector<int32_t>& data) override;

private:
	const GpuContext& m_ctx;
	GpuProgram m_histogramProgram;
	GpuProgram m_scanProgram;
	GpuProgram m_scatterProgram;
	GpuKernel m_histogramKernel;
	GpuKernel m_scanKernel;
	GpuKernel m_scatterKernel;
};