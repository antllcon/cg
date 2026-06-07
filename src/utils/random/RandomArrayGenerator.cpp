#include "RandomArrayGenerator.h"
#include <limits>
#include <random>

template<>
RandomArrayGenerator<uint32_t>::RandomArrayGenerator()
	: m_seed(std::random_device{}())
{
}

template<>
RandomArrayGenerator<uint32_t>::RandomArrayGenerator(uint64_t seed)
	: m_seed(seed)
{
}

template<>
std::vector<uint32_t> RandomArrayGenerator<uint32_t>::Generate(size_t count) const
{
	return Generate(count, 0u, std::numeric_limits<uint32_t>::max());
}

template<>
std::vector<uint32_t> RandomArrayGenerator<uint32_t>::Generate(
	size_t count,
	uint32_t min,
	uint32_t max) const
{
	std::mt19937 rng(static_cast<uint32_t>(m_seed));
	std::uniform_int_distribution<uint32_t> dist(min, max);

	std::vector<uint32_t> result(count);
	for (auto& val : result)
		val = dist(rng);

	return result;
}

template class RandomArrayGenerator<uint32_t>;