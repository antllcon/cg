#include "Framebuffer.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsFramebufferComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Фреймбуфер не прошёл проверку на полноту");
	}
}
} // namespace

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
{
	Allocate(width, height);
}

Framebuffer::~Framebuffer()
{
	Free();
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::Unbind() noexcept
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
	Free();
	Allocate(width, height);
}

uint32_t Framebuffer::GetTextureId() const noexcept
{
	return m_texture;
}

void Framebuffer::Allocate(uint32_t width, uint32_t height)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, static_cast<int>(width), static_cast<int>(height), 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
	AssertIsFramebufferComplete();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Free() noexcept
{
	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
		m_fbo = 0;
	}
	if (m_texture != 0)
	{
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
}
