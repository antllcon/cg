#pragma once

#include <cstdint>

class Framebuffer final
{
public:
	Framebuffer(uint32_t width, uint32_t height);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = delete;
	Framebuffer& operator=(Framebuffer&&) = delete;

	void Bind() const;
	static void Unbind() noexcept;
	void Resize(uint32_t width, uint32_t height);
	uint32_t GetTextureId() const noexcept;

private:
	void Allocate(uint32_t width, uint32_t height);
	void Free() noexcept;

	uint32_t m_fbo = 0;
	uint32_t m_texture = 0;
};
