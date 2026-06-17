#pragma once

#include <cstdint>

class GlMultiTexture final
{
public:
	GlMultiTexture();
	~GlMultiTexture() = default;

	bool IsSupported() const;
	void ActiveTexture(uint32_t unitIndex) const;
	void Coord(uint32_t unitIndex, float u, float v) const;

private:
	bool m_supported;
};
