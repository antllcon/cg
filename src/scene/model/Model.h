#pragma once

#include "ModelData.h"
#include "src/utils/Observer.h"

class Model final : public CObservable<ModelData>
{
public:
	Model() = default;
	~Model() override = default;

	void NextFractalType();
	void MoveCamera(float dx, float dy);
	void ZoomCamera(float delta);
	void ResetView();

	ModelData GetState() const;

protected:
	ModelData GetChangedData() const override;

private:
	ModelData m_data;
};