#include "Texture.h"
#include <glad/glad.h>
#include <utility>

Texture::Texture()
{
	glGenTextures(1, &m_textureId);
}

Texture::~Texture()
{
	Free();
}

void Texture::Bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UpdateData(uint32_t width, uint32_t height, uint32_t format, const void* data)
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Free() noexcept
{
	if (m_textureId != 0)
	{
		glDeleteTextures(1, &m_textureId);
		m_textureId = 0;
	}
}