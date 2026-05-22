#pragma once

#include "src/utils/image/Image.h"
#include <memory>

enum class AppState
{
	NoImage,
	ImageLoaded
};

namespace FilterConfig
{
inline constexpr uint8_t MIN_RADIUS = 0;
inline constexpr uint8_t MAX_RADIUS = 7;
} // namespace FilterConfig

struct ModelData
{
	AppState state = AppState::NoImage;
	std::shared_ptr<Image> image = nullptr;
	uint8_t medianRadius = FilterConfig::MIN_RADIUS;
};