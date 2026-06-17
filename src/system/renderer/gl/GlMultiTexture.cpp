#include "GlMultiTexture.h"
#include "src/system/renderer/gl/GlHeaders.h"
#include <GLFW/glfw3.h>

namespace
{
typedef void(APIENTRY* PfnActiveTextureArb)(GLenum texture);
typedef void(APIENTRY* PfnMultiTexCoord2fArb)(GLenum target, GLfloat s, GLfloat t);

constexpr GLenum TEXTURE0_ARB = 0x84C0;

PfnActiveTextureArb activeTextureProc = nullptr;
PfnMultiTexCoord2fArb multiTexCoord2fProc = nullptr;

template <typename Pfn>
Pfn LoadProc(const char* name)
{
	return reinterpret_cast<Pfn>(glfwGetProcAddress(name));
}
} // namespace

GlMultiTexture::GlMultiTexture()
	: m_supported(false)
{
	activeTextureProc = LoadProc<PfnActiveTextureArb>("glActiveTextureARB");
	multiTexCoord2fProc = LoadProc<PfnMultiTexCoord2fArb>("glMultiTexCoord2fARB");

	m_supported = activeTextureProc != nullptr && multiTexCoord2fProc != nullptr;
}

bool GlMultiTexture::IsSupported() const
{
	return m_supported;
}

void GlMultiTexture::ActiveTexture(uint32_t unitIndex) const
{
	if (activeTextureProc != nullptr)
	{
		activeTextureProc(TEXTURE0_ARB + unitIndex);
	}
}

void GlMultiTexture::Coord(uint32_t unitIndex, float u, float v) const
{
	if (multiTexCoord2fProc != nullptr)
	{
		multiTexCoord2fProc(TEXTURE0_ARB + unitIndex, u, v);
	}
}
