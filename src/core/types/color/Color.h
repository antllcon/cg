#pragma once

#include <cstdint>
#include <tuple>

class Color
{
public:
	Color();
	~Color() = default;

	static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	static Color FromFloat(float r, float g, float b, float a = 1.0f);

	std::tuple<float, float, float, float> GetAsFloats() const noexcept;
	std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> GetAsBytes() const noexcept;

	uint32_t ToHex() const noexcept;

	bool operator==(const Color& other) const noexcept;

private:
	Color(float r, float g, float b, float a);

	float m_r = 0.0f;
	float m_g = 0.0f;
	float m_b = 0.0f;
	float m_a = 1.0f;
};