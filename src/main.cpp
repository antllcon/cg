#include "Windows.h"
#include "benchmark/SortBenchmark.h"
#include "gpu/GpuContext.h"
#include "utils/console/ConsoleEncoding.h"
#include "utils/random/RandomArrayGenerator.h"
#include <iostream>

#ifdef _WIN32
extern "C" {
__declspec(dllexport) uint32_t NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

namespace
{
constexpr size_t ARRAY_SIZE = 10'000'000;
constexpr uint64_t RANDOM_SEED = 228;
} // namespace

int main()
{
	ConsoleEncoding encoding;

	try
	{
		RandomArrayGenerator<uint32_t> generator(RANDOM_SEED);
		auto udata = generator.Generate(ARRAY_SIZE);
		std::vector<int32_t> data(udata.begin(), udata.end());

		GpuContext ctx;
		SortBenchmark benchmark(ctx);
		benchmark.Run(data);
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Error]\t" << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}