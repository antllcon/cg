#pragma once

#include <cstdint>
#include <vector>

template<typename T>
class RandomArrayGenerator
{
public:
	RandomArrayGenerator();
	explicit RandomArrayGenerator(uint64_t seed);

	std::vector<T> Generate(size_t count) const;
	std::vector<T> Generate(size_t count, T min, T max) const;

private:
	uint64_t m_seed;
};