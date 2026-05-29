#pragma once

#include "src/scene/model/ModelData.h"
#include "src/utils/color/Color.h"
#include <cstdint>

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void SetViewport(uint32_t width, uint32_t height) = 0;
	virtual void SetClearColor(const Color& color) = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;

	virtual void RenderFrame(const ModelData& data) = 0;
};