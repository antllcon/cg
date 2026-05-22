#pragma once

#include <cstdint>
#include <tuple>

class Color
{
public:
	static constexpr uint8_t RGB_MAX = 255;
	static constexpr float FLOAT_MAX = 1.0f;

	Color();
	~Color() = default;

	static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = RGB_MAX);
	static Color FromFloat(float r, float g, float b, float a = FLOAT_MAX);

	std::tuple<float, float, float, float> GetAsFloats() const;
	std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> GetAsBytes() const;

	uint8_t GetRAsByte() const;
	uint8_t GetGAsByte() const;
	uint8_t GetBAsByte() const;
	uint8_t GetAAsByte() const;

	uint32_t ToHex() const;

	bool operator==(const Color& other) const noexcept;

private:
	Color(float r, float g, float b, float a);

	float m_r = 0.0f;
	float m_g = 0.0f;
	float m_b = 0.0f;
	float m_a = 1.0f;
};