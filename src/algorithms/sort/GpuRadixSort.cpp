#include "GpuRadixSort.h"
#include "gpu/GpuBuffer.h"
#include <algorithm>
#include <cstdint>

namespace
{
constexpr size_t LOCAL_SIZE = 256;
constexpr size_t RADIX_SIZE = 256;
constexpr uint32_t NUM_PASSES = 4;

std::vector<uint32_t> ToUnsigned(const std::vector<int32_t>& data)
{
	std::vector<uint32_t> result(data.size());
	for (size_t i = 0; i < data.size(); i++)
		result[i] = static_cast<uint32_t>(data[i]) ^ 0x80000000u;
	return result;
}

void ToSigned(const std::vector<uint32_t>& src, std::vector<int32_t>& dst)
{
	for (size_t i = 0; i < dst.size(); i++)
		dst[i] = static_cast<int32_t>(src[i] ^ 0x80000000u);
}
} // namespace

GpuRadixSort::GpuRadixSort(const GpuContext& ctx)
	: m_ctx(ctx)
	, m_histogramProgram(ctx, "static/kernels/radix_histogram.cl")
	, m_scanProgram(ctx, "static/kernels/radix_prefix_scan.cl")
	, m_scatterProgram(ctx, "static/kernels/radix_scatter.cl")
	, m_histogramKernel(m_histogramProgram, "radix_histogram")
	, m_scanKernel(m_scanProgram, "radix_prefix_scan")
	, m_scatterKernel(m_scatterProgram, "radix_scatter")
{
}

std::chrono::nanoseconds GpuRadixSort::Sort(std::vector<int32_t>& data)
{
	const size_t n = data.size();
	const size_t numGroups = (n + LOCAL_SIZE - 1) / LOCAL_SIZE;
	const size_t paddedSize = numGroups * LOCAL_SIZE;

	std::vector<uint32_t> udata = ToUnsigned(data);
	udata.resize(paddedSize, 0u);

	GpuBuffer<uint32_t> bufA(m_ctx, udata);
	GpuBuffer<uint32_t> bufB(m_ctx, paddedSize);
	GpuBuffer<uint32_t> histBuf(m_ctx, numGroups * RADIX_SIZE);

	auto start = std::chrono::high_resolution_clock::now();

	for (uint32_t pass = 0; pass < NUM_PASSES; pass++)
	{
		uint32_t shift = pass * 8;
		uint32_t ucount = static_cast<uint32_t>(n);
		uint32_t ugroups = static_cast<uint32_t>(numGroups);

		m_histogramKernel.SetArg(0, bufA);
		m_histogramKernel.SetArg(1, histBuf);
		m_histogramKernel.SetArg(2, shift);
		m_histogramKernel.SetArg(3, ucount);
		m_histogramKernel.Dispatch(m_ctx, paddedSize, LOCAL_SIZE);

		m_scanKernel.SetArg(0, histBuf);
		m_scanKernel.SetArg(1, ugroups);
		m_scanKernel.Dispatch(m_ctx, RADIX_SIZE, RADIX_SIZE);

		m_scatterKernel.SetArg(0, bufA);
		m_scatterKernel.SetArg(1, bufB);
		m_scatterKernel.SetArg(2, histBuf);
		m_scatterKernel.SetArg(3, shift);
		m_scatterKernel.SetArg(4, ucount);
		m_scatterKernel.Dispatch(m_ctx, paddedSize, LOCAL_SIZE);

		std::swap(bufA, bufB);
	}

	m_ctx.Finish();
	auto end = std::chrono::high_resolution_clock::now();

	std::vector<uint32_t> result;
	bufA.Download(m_ctx, result);
	result.resize(n);

	data.resize(n);
	ToSigned(result, data);

	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}
