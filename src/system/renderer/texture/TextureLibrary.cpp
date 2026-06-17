#include "TextureLibrary.h"
#include "src/system/AppConfig.h"
#include <array>
#include <iostream>
#include <utility>
#include <vector>

namespace
{
constexpr uint32_t FALLBACK_SIZE = 64;
constexpr uint32_t FALLBACK_CELL = 8;

Texture MakeFallbackChecker()
{
	std::vector<unsigned char> pixels(static_cast<size_t>(FALLBACK_SIZE) * FALLBACK_SIZE * 3);

	for (uint32_t y = 0; y < FALLBACK_SIZE; ++y)
	{
		for (uint32_t x = 0; x < FALLBACK_SIZE; ++x)
		{
			const bool isLight = ((x / FALLBACK_CELL) + (y / FALLBACK_CELL)) % 2 == 0;
			const unsigned char value = isLight ? 200 : 90;
			const size_t index = (static_cast<size_t>(y) * FALLBACK_SIZE + x) * 3;

			pixels[index] = value;
			pixels[index + 1] = value;
			pixels[index + 2] = value;
		}
	}

	return Texture(FALLBACK_SIZE, FALLBACK_SIZE, 3, pixels.data());
}

void LoadInto(std::map<TextureKey, Texture>& textures, TextureKey key, const char* path)
{
	try
	{
		textures.emplace(key, Texture(path));
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Texture] \t" << e.what() << " — используется заглушка" << std::endl;
		textures.emplace(key, MakeFallbackChecker());
	}
}
} // namespace

TextureLibrary::TextureLibrary()
{
	LoadInto(m_textures, TextureKey::Wall1, TextureConfig::WALL_1);
	LoadInto(m_textures, TextureKey::Wall2, TextureConfig::WALL_2);
	LoadInto(m_textures, TextureKey::Wall3, TextureConfig::WALL_3);
	LoadInto(m_textures, TextureKey::Wall4, TextureConfig::WALL_4);
	LoadInto(m_textures, TextureKey::Wall5, TextureConfig::WALL_5);
	LoadInto(m_textures, TextureKey::Wall6, TextureConfig::WALL_6);
	LoadInto(m_textures, TextureKey::Floor, TextureConfig::FLOOR);
	LoadInto(m_textures, TextureKey::Ceiling, TextureConfig::CEILING);
	LoadInto(m_textures, TextureKey::Shadow, TextureConfig::SHADOW);
	LoadInto(m_textures, TextureKey::Sky, TextureConfig::SKY);
}

bool TextureLibrary::Bind(TextureKey key) const
{
	const auto it = m_textures.find(key);
	if (it == m_textures.end())
	{
		return false;
	}

	it->second.Bind();
	return true;
}
