#pragma once

#include "Texture.h"
#include "TextureKey.h"
#include <map>

class TextureLibrary final
{
public:
	TextureLibrary();
	~TextureLibrary() = default;

	TextureLibrary(const TextureLibrary&) = delete;
	TextureLibrary& operator=(const TextureLibrary&) = delete;

	bool Bind(TextureKey key) const;

private:
	std::map<TextureKey, Texture> m_textures;
};
