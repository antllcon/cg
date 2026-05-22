#include "Model.h"
#include <filesystem>
#include <stdexcept>

namespace
{
void AssertIsRadiusValid(uint8_t radius)
{
	if (radius > FilterConfig::MAX_RADIUS)
	{
		throw std::runtime_error("Радиус медианного фильтра должен быть от 0 до 7");
	}
}

void AssertIsFileExists(const std::string& path)
{
	if (!std::filesystem::exists(path))
	{
		throw std::runtime_error("Указанный файл изображения не найден");
	}
}
} // namespace

void Model::LoadImage(const std::string& path)
{
	AssertIsFileExists(path);

	m_data.image = std::make_shared<Image>(path);
	m_data.state = AppState::ImageLoaded;
	m_data.medianRadius = FilterConfig::MIN_RADIUS;

	NotifyObservers();
}

void Model::SaveImage(const std::string& path) const
{
	if (m_data.image)
	{
		m_data.image->Save(path);
	}
}

void Model::CloseImage()
{
	m_data.image = nullptr;
	m_data.state = AppState::NoImage;

	NotifyObservers();
}

void Model::SetMedianRadius(uint8_t radius)
{
	AssertIsRadiusValid(radius);

	m_data.medianRadius = radius;

	NotifyObservers();
}

uint8_t Model::GetMedianRadius() const
{
	return m_data.medianRadius;
}

ModelData Model::GetState() const
{
	return m_data;
}

ModelData Model::GetChangedData() const
{
	return m_data;
}