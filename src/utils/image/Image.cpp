#define _CRT_SECURE_NO_WARNINGS
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb/stb_imagewrite.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <stdexcept>

namespace
{
std::string GetExtensionToLower(const std::filesystem::path& path)
{
	std::string ext = path.extension().string();
	for (auto& ch : ext)
	{
		ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
	}
	return ext;
}

void AssertIsFileExists(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::invalid_argument("Файл не найден: " + path.string());
	}
}

void AssertIsImageFile(const std::filesystem::path& path)
{
	const auto ext = GetExtensionToLower(path);

	if (!Image::EXTENSIONS.contains(ext))
	{
		throw std::invalid_argument("Файл не является изображением: " + path.string());
	}
}

void AssertIsLoaded(const unsigned char* data)
{
	if (!data)
	{
		throw std::runtime_error(std::string("Ошибка загрузки изображения: ") + stbi_failure_reason());
	}
}

void AssertIsDimensionsValid(const uint32_t width, const uint32_t height, const uint8_t channels)
{
	if (width <= 0 || height <= 0 || channels <= 0)
	{
		throw std::runtime_error("Некорректные размеры изображения");
	}
}

void AssertIsRawDataProvided(const unsigned char* data)
{
	if (!data)
	{
		throw std::invalid_argument("Передан пустой указатель на данные пикселей");
	}
}

void AssertIsMemoryAllocated(const unsigned char* data)
{
	if (!data)
	{
		throw std::runtime_error("Не удалось выделить память под изображение");
	}
}

void AssertIsDataValidForSave(const unsigned char* data)
{
	if (!data)
	{
		throw std::runtime_error("Нет данных для сохранения (изображение пустое)");
	}
}

void AssertIsFormatSupported(bool isSupported, const std::string& ext)
{
	if (!isSupported)
	{
		throw std::invalid_argument("Неподдерживаемый формат для сохранения: " + ext);
	}
}

void AssertIsImageSaved(int success, const std::filesystem::path& path)
{
	if (!success)
	{
		throw std::runtime_error("Не удалось сохранить изображение по пути: " + path.string());
	}
}
} // namespace

Image::Image(const std::string& path)
{
	Load(path);
}

Image::Image(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data)
	: m_width(width)
	, m_height(height)
	, m_channels(channels)
{
	AssertIsDimensionsValid(width, height, channels);
	AssertIsRawDataProvided(data);

	const size_t bytesCount = GetPixelCount() * m_channels;

	m_data = static_cast<unsigned char*>(std::malloc(bytesCount));
	AssertIsMemoryAllocated(m_data);

	std::memcpy(m_data, data, bytesCount);
}

Image::~Image()
{
	Free();
}

Image::Image(Image&& other) noexcept
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_channels = other.m_channels;
	m_data = other.m_data;

	other.m_data = nullptr;
}

Image& Image::operator=(Image&& other) noexcept
{
	if (this != &other)
	{
		Free();

		m_width = other.m_width;
		m_height = other.m_height;
		m_channels = other.m_channels;
		m_data = other.m_data;

		other.m_data = nullptr;
	}
	return *this;
}

void Image::Save(const std::string& path) const
{
	AssertIsDataValidForSave(m_data);

	const auto ext = GetExtensionToLower(path);
	int success = 0;

	if (ext == ".png")
	{
		success = stbi_write_png(path.c_str(), m_width, m_height, m_channels, m_data, m_width * m_channels);
	}
	else if (ext == ".jpg" || ext == ".jpeg")
	{
		success = stbi_write_jpg(path.c_str(), m_width, m_height, m_channels, m_data, 100);
	}
	else
	{
		AssertIsFormatSupported(false, ext);
	}

	AssertIsImageSaved(success, path);
}

bool Image::IsRGB() const
{
	return m_channels == 3u;
}

uint32_t Image::GetWidth() const
{
	return m_width;
}

uint32_t Image::GetHeight() const
{
	return m_height;
}

uint8_t Image::GetChannels() const
{
	return m_channels;
}

uint64_t Image::GetPixelCount() const
{
	return m_width * m_height;
}

const unsigned char* Image::GetData() const
{
	return m_data;
}

unsigned char* Image::GetData()
{
	return m_data;
}

void Image::Load(const std::string& path)
{
	AssertIsFileExists(path);
	AssertIsImageFile(path);

	int width = 0;
	int height = 0;
	int channels = 0;

	auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	AssertIsLoaded(data);
	AssertIsDimensionsValid(
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height),
		static_cast<uint8_t>(channels));

	m_width = static_cast<uint32_t>(width);
	m_height = static_cast<uint32_t>(height);
	m_channels = static_cast<uint8_t>(channels);
	m_data = data;
}

void Image::Free() noexcept
{
	if (m_data)
	{
		stbi_image_free(m_data);
		m_data = nullptr;
	}
}