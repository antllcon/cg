#pragma once

#include "src/scene/model/ModelData.h"
#include "src/utils/color/Color.h"

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void SetClearColor(const Color& color) = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;

	virtual void DrawProcedural(const ModelData& data) = 0;
};