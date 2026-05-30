#pragma once

#include <cstdint>

struct FramebufferConfig
{
	uint32_t width = 1;
	uint32_t height = 1;
	bool hasDepthBuffer = true;
	bool useClampToEdge = false;
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

	uint32_t GetColorTexture() const;

private:
	uint32_t m_fbo = 0;
	uint32_t m_colorTex = 0;
	uint32_t m_depthRbo = 0;
};