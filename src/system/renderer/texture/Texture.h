#pragma once

#include <cstdint>
#include <string>

class Texture final
{
public:
	explicit Texture(const std::string& path);
	Texture(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	void Bind() const;
	uint32_t GetId() const;

private:
	void Upload(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data);
	void Release() noexcept;

	uint32_t m_id = 0;
};
