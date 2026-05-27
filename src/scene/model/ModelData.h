#pragma once

#include <cstdint>

enum class FractalType : uint8_t
{
	Mandelbrot,
	Julia,
	BurningShip,
	Count
};

inline FractalType& operator++(FractalType& type)
{
	type = static_cast<FractalType>((static_cast<uint8_t>(type) + 1) % static_cast<uint8_t>(FractalType::Count));
	return type;
}

namespace FractalDefaults
{
inline constexpr float OFFSET_X = 0.0f;
inline constexpr float OFFSET_Y = 0.0f;
inline constexpr float ZOOM = 1.0f;
} // namespace FractalDefaults

struct ModelData
{
	FractalType type = FractalType::Mandelbrot;
	float offsetX = FractalDefaults::OFFSET_X;
	float offsetY = FractalDefaults::OFFSET_Y;
	float zoom = FractalDefaults::ZOOM;
};