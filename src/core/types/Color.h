#pragma once
#include <cstdint>

struct Color
{
	uint8_t r{0};
	uint8_t g{0};
	uint8_t b{0};
	uint8_t a{255};

	bool operator==(const Color&) const = default;
};