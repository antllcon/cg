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

	void SetMedianRadius(uint8_t radius);
	uint8_t GetMedianRadius() const;

	ModelData GetState() const;

protected:
	ModelData GetChangedData() const override;

private:
	ModelData m_data;
};