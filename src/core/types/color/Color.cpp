#include "Color.h"
#include <cmath>
#include <limits>
#include <stdexcept>

namespace
{
void AssertIsNormalized(float value)
{
	if (value < 0.0f || value > 1.0f)
	{
		throw std::runtime_error("Значение цветового канала должно быть в диапазоне от 0.0 до 1.0");
	}
}

float ConvertByteToFloat(uint8_t value)
{
	return static_cast<float>(value) / 255.0f;
}

uint8_t ConvertFloatToByte(float value)
{
	return static_cast<uint8_t>(std::round(value * 255.0f));
}

bool AreFloatsEqual(float a, float b)
{
	return std::abs(a - b) < std::numeric_limits<float>::epsilon();
}
} // namespace

Color::Color()
{
}

Color::Color(float r, float g, float b, float a)
	: m_r(r)
	, m_g(g)
	, m_b(b)
	, m_a(a)
{
}

Color Color::FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	return Color(
		ConvertByteToFloat(r),
		ConvertByteToFloat(g),
		ConvertByteToFloat(b),
		ConvertByteToFloat(a));
}

Color Color::FromFloat(float r, float g, float b, float a)
{
	AssertIsNormalized(r);
	AssertIsNormalized(g);
	AssertIsNormalized(b);
	AssertIsNormalized(a);

	return Color(r, g, b, a);
}

std::tuple<float, float, float, float> Color::GetAsFloats() const noexcept
{
	return {m_r, m_g, m_b, m_a};
}

std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> Color::GetAsBytes() const noexcept
{
	return {
		ConvertFloatToByte(m_r),
		ConvertFloatToByte(m_g),
		ConvertFloatToByte(m_b),
		ConvertFloatToByte(m_a)};
}

uint32_t Color::ToHex() const noexcept
{
	auto [r, g, b, a] = GetAsBytes();

	uint32_t r32 = r;
	uint32_t g32 = g;
	uint32_t b32 = b;
	uint32_t a32 = a;

	return r32 << 24 | g32 << 16 | b32 << 8 | a32;
}

bool Color::operator==(const Color& other) const noexcept
{
	return AreFloatsEqual(m_r, other.m_r)
		&& AreFloatsEqual(m_g, other.m_g)
		&& AreFloatsEqual(m_b, other.m_b)
		&& AreFloatsEqual(m_a, other.m_a);
}