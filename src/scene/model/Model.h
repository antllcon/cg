#pragma once

#include "ModelData.h"
#include "src/utils/Observer.h"
#include <string>

class Model final : public CObservable<ModelData>
{
public:
	Model() = default;
	~Model() override = default;

	void LoadImage(const std::string& path);
	void SaveImage(const std::string& path) const;
	void CloseImage();

	void ToggleFilter();
	void IncreaseFilterRadius();
	void DecreaseFilterRadius();

	ModelData GetState() const;

protected:
	ModelData GetChangedData() const override;

private:
	ModelData m_data;
	uint8_t m_cachedMedianRadius = MedianFilterSettings::MIN_RADIUS;
	uint8_t m_cachedGaussianRadius = GaussianBlurSettings::MIN_RADIUS;
};