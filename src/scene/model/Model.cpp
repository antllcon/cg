#include "Model.h"
#include <filesystem>
#include <stdexcept>

namespace
{
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
	m_data.filter = NoFilterSettings{};

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
	m_data.filter = NoFilterSettings{};

	NotifyObservers();
}

void Model::ToggleFilter()
{
	if (std::holds_alternative<NoFilterSettings>(m_data.filter))
	{
		MedianFilterSettings settings;
		settings.radius = m_cachedMedianRadius;
		m_data.filter = settings;
	}
	else
	{
		if (const auto* medianSettings = std::get_if<MedianFilterSettings>(&m_data.filter))
		{
			m_cachedMedianRadius = medianSettings->radius;
		}

		m_data.filter = NoFilterSettings{};
	}

	NotifyObservers();
}

void Model::IncreaseFilterRadius()
{
	if (auto* medianSettings = std::get_if<MedianFilterSettings>(&m_data.filter))
	{
		if (medianSettings->radius < MedianFilterSettings::MAX_RADIUS)
		{
			medianSettings->radius++;
			NotifyObservers();
		}
	}
}

void Model::DecreaseFilterRadius()
{
	if (auto* medianSettings = std::get_if<MedianFilterSettings>(&m_data.filter))
	{
		if (medianSettings->radius > MedianFilterSettings::MIN_RADIUS)
		{
			medianSettings->radius--;
			NotifyObservers();
		}
	}
}

ModelData Model::GetState() const
{
	return m_data;
}

ModelData Model::GetChangedData() const
{
	return m_data;
}