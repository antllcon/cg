#pragma once
#include "IRenderer.h"
#include "src/utils/image/Image.h"

class OpenGLRenderer : public IRenderer
{
public:
	~OpenGLRenderer() override;

	void SetClearColor(const Color& color) override;
	void Clear() override;
	void Display() override;
	void DrawImageFiltered(const std::shared_ptr<Image>& image, uint8_t medianRadius) override;
};