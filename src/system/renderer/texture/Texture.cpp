#include "Texture.h"
#include "src/system/renderer/gl/GlHeaders.h"
#include "src/utils/image/Image.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace
{
void AssertIsChannelsSupported(uint8_t channels)
{
	if (channels != 1 && channels != 3 && channels != 4)
	{
		throw std::runtime_error("Поддерживаются только текстуры с 1, 3 или 4 каналами");
	}
}

GLenum ResolveFormat(uint8_t channels)
{
	switch (channels)
	{
	case 1: return GL_LUMINANCE;
	case 4: return GL_RGBA;
	default: return GL_RGB;
	}
}

std::vector<unsigned char> FlipVertically(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data)
{
	const size_t rowSize = static_cast<size_t>(width) * channels;
	std::vector<unsigned char> flipped(rowSize * height);

	for (uint32_t y = 0; y < height; ++y)
	{
		const unsigned char* source = data + static_cast<size_t>(height - 1 - y) * rowSize;
		std::copy(source, source + rowSize, flipped.begin() + static_cast<ptrdiff_t>(y * rowSize));
	}

	return flipped;
}
} // namespace

Texture::Texture(const std::string& path)
{
	const Image image(path);
	Upload(image.GetWidth(), image.GetHeight(), image.GetChannels(), image.GetData());
}

Texture::Texture(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data)
{
	Upload(width, height, channels, data);
}

Texture::~Texture()
{
	Release();
}

Texture::Texture(Texture&& other) noexcept
	: m_id(other.m_id)
{
	other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this != &other)
	{
		Release();
		m_id = other.m_id;
		other.m_id = 0;
	}
	return *this;
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(m_id));
}

uint32_t Texture::GetId() const
{
	return m_id;
}

void Texture::Upload(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data)
{
	AssertIsChannelsSupported(channels);

	const GLenum format = ResolveFormat(channels);
	const std::vector<unsigned char> pixels = FlipVertically(width, height, channels, data);

	GLuint id = 0;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluBuild2DMipmaps(
		GL_TEXTURE_2D,
		static_cast<GLint>(format),
		static_cast<GLsizei>(width),
		static_cast<GLsizei>(height),
		format,
		GL_UNSIGNED_BYTE,
		pixels.data());

	m_id = id;
}

void Texture::Release() noexcept
{
	if (m_id != 0)
	{
		const GLuint id = static_cast<GLuint>(m_id);
		glDeleteTextures(1, &id);
		m_id = 0;
	}
}
