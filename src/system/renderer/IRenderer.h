#pragma once

#include "../../scene/model/FilterTypes.h"
#include "src/utils/color/Color.h"
#include "src/utils/image/Image.h"
#include <memory>

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void SetViewport(uint32_t width, uint32_t height) = 0;
	virtual void SetClearColor(const Color& color) = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;
	virtual void DrawImage(const std::shared_ptr<Image>& image, const FilterSettings& settings) = 0;
};