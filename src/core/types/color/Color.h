#pragma once

#include <cstdint>

class Color
{
public:
	Color();
	~Color() = default;

	static Color FromRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
	static Color FromFloat(float r, float g, float b, float a = 1.0f);

	float GetR() const noexcept;
	float GetG() const noexcept;
	float GetB() const noexcept;
	float GetA() const noexcept;

	uint8_t GetRAsByte() const noexcept;
	uint8_t GetGAsByte() const noexcept;
	uint8_t GetBAsByte() const noexcept;
	uint8_t GetAAsByte() const noexcept;

	uint32_t ToHex() const noexcept;

	bool operator==(const Color&) const = default;

private:
	Color(float r, float g, float b, float a);

	float m_r;
	float m_g;
	float m_b;
	float m_a;
};