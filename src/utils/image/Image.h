#pragma once

#include <ostream>
#include <set>
#include <string>
#include <vector>

class Image
{
public:
	inline static const std::set<std::string> EXTENSIONS = {".png", ".jpg", ".jpeg"};
	inline static const std::vector<std::string> DIALOG_FILTER = {"*.png *.jpg *.jpeg"};

	explicit Image(const std::string& path);
	Image(uint32_t width, uint32_t height, uint8_t channels, const unsigned char* data);
	~Image();

	Image(const Image&) = delete;
	Image& operator=(const Image&) = delete;
	Image(Image&& other) noexcept;
	Image& operator=(Image&& other) noexcept;

	void Save(const std::string& path) const;

	bool IsRGB() const;

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	uint8_t GetChannels() const;
	uint64_t GetPixelCount() const;

	const unsigned char* GetData() const;
	unsigned char* GetData();

private:
	void Load(const std::string& path);
	void Free() noexcept;

private:
	uint32_t m_width = 0;
	uint32_t m_height = 0;
	uint8_t m_channels = 0;
	unsigned char* m_data = nullptr;
};