#pragma once

#include <array>
#include <cstdint>

struct FramebufferConfig
{
	uint32_t width = 1;
	uint32_t height = 1;
	bool hasDepthBuffer = true;
	bool useClampToEdge = false;
	uint8_t colorAttachmentCount = 1;
};

class Framebuffer final
{
public:
	explicit Framebuffer(const FramebufferConfig& config);
	~Framebuffer();

	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&&) = delete;
	Framebuffer& operator=(Framebuffer&&) = delete;

	void Bind() const;
	static void Unbind();

	uint32_t GetColorTexture(uint8_t index = 0) const;

private:
	uint32_t m_fbo = 0;
	std::array<uint32_t, 2> m_colorTextures = { 0, 0 };
	uint32_t m_depthRbo = 0;
};