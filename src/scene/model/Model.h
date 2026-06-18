#pragma once

#include "ModelData.h"
#include "src/utils/Observer.h"

class Model final : public CObservable<ModelData>
{
public:
	Model() = default;
	~Model() override = default;

	ModelData GetState() const;

protected:
	ModelData GetChangedData() const override;

private:
	ModelData m_data;
};