#include "Framebuffer.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
void AssertIsGenerated(uint32_t id)
{
	if (id == 0)
	{
		throw std::runtime_error("Не удалось сгенерировать объект фреймбуфера или его компоненты");
	}
}

void AssertIsFramebufferComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Ошибка инициализации: фреймбуфер собран некорректно");
	}
}
} // namespace

Framebuffer::Framebuffer(const FramebufferConfig& config)
{
	glGenFramebuffers(1, &m_fbo);
	AssertIsGenerated(m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(1, &m_colorTex);
	AssertIsGenerated(m_colorTex);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, config.width, config.height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (config.useClampToEdge)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex, 0);

	if (config.hasDepthBuffer)
	{
		glGenRenderbuffers(1, &m_depthRbo);
		AssertIsGenerated(m_depthRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, config.width, config.height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRbo);
	}

	AssertIsFramebufferComplete();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	if (m_depthRbo != 0)
	{
		glDeleteRenderbuffers(1, &m_depthRbo);
	}

	if (m_colorTex != 0)
	{
		glDeleteTextures(1, &m_colorTex);
	}

	if (m_fbo != 0)
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
}

void Framebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t Framebuffer::GetColorTexture() const
{
	return m_colorTex;
}