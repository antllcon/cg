#include "Framebuffer.h"
#include <glad/glad.h>
#include <stdexcept>

namespace
{
constexpr std::array<GLenum, 2> DRAW_BUFFERS = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

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

void AssertIsAttachmentCountValid(uint8_t count)
{
	if (count == 0 || count > 2)
	{
		throw std::invalid_argument("Количество цветовых должно быть от 1 до 2");
	}
}

uint32_t CreateColorTexture(uint32_t width, uint32_t height, bool useClampToEdge)
{
	uint32_t textureIndex = 0;
	glGenTextures(1, &textureIndex);
	AssertIsGenerated(textureIndex);

	glBindTexture(GL_TEXTURE_2D, textureIndex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (useClampToEdge)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	return textureIndex;
}
} // namespace

Framebuffer::Framebuffer(const FramebufferConfig& config)
{
	AssertIsAttachmentCountValid(config.colorAttachmentCount);

	glGenFramebuffers(1, &m_fbo);
	AssertIsGenerated(m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	for (uint8_t i = 0; i < config.colorAttachmentCount; ++i)
	{
		m_colorTextures[i] = CreateColorTexture(config.width, config.height, config.useClampToEdge);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorTextures[i], 0);
	}

	if (config.colorAttachmentCount > 1)
	{
		glDrawBuffers(config.colorAttachmentCount, DRAW_BUFFERS.data());
	}

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

	for (uint32_t tex : m_colorTextures)
	{
		if (tex != 0)
		{
			glDeleteTextures(1, &tex);
		}
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

uint32_t Framebuffer::GetColorTexture(uint8_t index) const
{
	return m_colorTextures[index];
}