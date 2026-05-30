#pragma once

#include <cstdint>

class Texture final
{
public:
	Texture();
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) = delete;
	Texture& operator=(Texture&& other) = delete;

	void Bind(uint32_t slot = 0) const;
	void Unbind() const;

	void UpdateData(uint32_t width, uint32_t height, uint32_t format, const void* data);

private:
	void Free() noexcept;

	uint32_t m_textureId = 0;
};