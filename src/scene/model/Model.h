#pragma once

#include "ModelData.h"
#include "src/utils/Observer.h"

class Model final : public CObservable<ModelData>
{
public:
	Model();
	~Model() override = default;

	void MoveCamera(float dForward, float dRight, float dUp);
	void RotateCamera(float dYaw, float dPitch);
	void Update(float dt);

	ModelData GetState() const;

protected:
	ModelData GetChangedData() const override;

private:
	void InitScene();

	ModelData m_data;
	float m_totalTime;
};