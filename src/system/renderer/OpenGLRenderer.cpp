#include "OpenGLRenderer.h"
#include <glad/glad.h>

void OpenGLRenderer::SetClearColor(const Color& color)
{
	auto [r, g, b, a] = color.GetAsFloats();
	glClearColor(r, g, b, a);
}

void OpenGLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Display()
{
	glFlush();
}

void OpenGLRenderer::DrawImageFiltered(const std::shared_ptr<Image>& image, uint8_t medianRadius)
{
	(void)image;
	(void)medianRadius;
}