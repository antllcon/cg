#include "SortBenchmark.h"
#include "algorithms/sort/CpuParallelSort.h"
#include "algorithms/sort/GpuRadixSort.h"
#include <iomanip>
#include <iostream>
#include <stdexcept>

namespace
{
double ToMilliseconds(std::chrono::nanoseconds ns)
{
	return static_cast<double>(ns.count()) / 1'000'000.0;
}

void AssertIsCorrect(const std::vector<int32_t>& gpu, const std::vector<int32_t>& cpu)
{
	if (gpu != cpu)
		throw std::runtime_error("Результаты GPU и CPU сортировок не совпадают");
}

void PrintRow(const std::string& name, double ms)
{
	std::cout
		<< std::left << std::setw(24) << name
		<< std::right << std::setw(12) << std::fixed << std::setprecision(3) << ms << " ms"
		<< std::endl;
}

void PrintSpeedup(double gpuMs, double cpuMs)
{
	double ratio = gpuMs / cpuMs;
	std::string winner = (ratio < 1.0) ? "GPU" : "CPU";
	double factor = (ratio < 1.0) ? (1.0 / ratio) : ratio;

	std::cout
		<< std::left << std::setw(24) << "Ускорение"
		<< std::right << std::setw(12) << std::fixed << std::setprecision(2)
		<< factor << "x (" << winner << " быстрее)"
		<< std::endl;
}
} // namespace

SortBenchmark::SortBenchmark(const GpuContext& ctx)
	: m_ctx(ctx)
{
}

void SortBenchmark::Run(const std::vector<int32_t>& data) const
{
	std::cout << "\n=== Radix Sort Benchmark ===" << std::endl;
	std::cout << "Размер массива: " << data.size() << " элементов\n" << std::endl;

	std::cout
		<< std::left << std::setw(24) << "Алгоритм"
		<< std::right << std::setw(14) << "Время"
		<< std::endl;
	std::cout << std::string(40, '-') << std::endl;

	std::vector<int32_t> gpuData = data;
	GpuRadixSort gpuSorter(m_ctx);
	auto gpuTime = gpuSorter.Sort(gpuData);
	PrintRow("GPU Radix Sort", ToMilliseconds(gpuTime));

	std::vector<int32_t> cpuData = data;
	CpuParallelSort cpuSorter;
	auto cpuTime = cpuSorter.Sort(cpuData);
	PrintRow("CPU std::sort", ToMilliseconds(cpuTime));

	std::cout << std::string(40, '-') << std::endl;
	PrintSpeedup(ToMilliseconds(gpuTime), ToMilliseconds(cpuTime));

	AssertIsCorrect(gpuData, cpuData);
	std::cout << "\nРезультат: OK, оба результата совпадают" << std::endl;
}