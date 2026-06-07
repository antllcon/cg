#pragma once

#include "src/utils/image/Image.h"
#include <cstdint>
#include <memory>
#include <variant>

struct NoFilterSettings
{
};

struct MedianFilterSettings
{
	static constexpr uint8_t MIN_RADIUS = 0;
	static constexpr uint8_t MAX_RADIUS = 7;

	uint8_t radius = MIN_RADIUS;
};

struct GaussianBlurSettings
{
	static constexpr uint8_t MIN_RADIUS = 1;
	static constexpr uint8_t MAX_RADIUS = 40;

	uint8_t radius = MIN_RADIUS;
};

using FilterSettings = std::variant<NoFilterSettings, MedianFilterSettings, GaussianBlurSettings>;

struct ModelData
{
	std::shared_ptr<Image> image = nullptr;
	FilterSettings filter = NoFilterSettings{};
};