#include "Color.h"

#include <cmath>
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
} // namespace

Color::Color()
	: m_r(0.0f)
	, m_g(0.0f)
	, m_b(0.0f)
	, m_a(1.0f)
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
	return Color(ConvertByteToFloat(r), ConvertByteToFloat(g), ConvertByteToFloat(b), ConvertByteToFloat(a));
}

Color Color::FromFloat(float r, float g, float b, float a)
{
	AssertIsNormalized(r);
	AssertIsNormalized(g);
	AssertIsNormalized(b);
	AssertIsNormalized(a);

	return Color(r, g, b, a);
}

float Color::GetR() const noexcept
{
	return m_r;
}

float Color::GetG() const noexcept
{
	return m_g;
}

float Color::GetB() const noexcept
{
	return m_b;
}

float Color::GetA() const noexcept
{
	return m_a;
}

uint8_t Color::GetRAsByte() const noexcept
{
	return ConvertFloatToByte(m_r);
}

uint8_t Color::GetGAsByte() const noexcept
{
	return ConvertFloatToByte(m_g);
}

uint8_t Color::GetBAsByte() const noexcept
{
	return ConvertFloatToByte(m_b);
}

uint8_t Color::GetAAsByte() const noexcept
{
	return ConvertFloatToByte(m_a);
}

uint32_t Color::ToHex() const noexcept
{
	uint32_t r = GetRAsByte();
	uint32_t g = GetGAsByte();
	uint32_t b = GetBAsByte();
	uint32_t a = GetAAsByte();

	return (r << 24) | (g << 16) | (b << 8) | a;
}