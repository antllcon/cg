#include "SkySphere.h"
#include "src/system/AppConfig.h"
#include "src/system/renderer/gl/GlHeaders.h"
#include "src/system/renderer/texture/TextureLibrary.h"
#include <stdexcept>

namespace
{
void AssertIsQuadricValid(const void* quadric)
{
	if (quadric == nullptr)
	{
		throw std::runtime_error("Не удалось создать сферу неба");
	}
}
} // namespace

SkySphere::SkySphere()
	: m_quadric(gluNewQuadric())
{
	AssertIsQuadricValid(m_quadric);

	GLUquadric* quadric = static_cast<GLUquadric*>(m_quadric);
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_NONE);
	gluQuadricOrientation(quadric, GLU_INSIDE);
}

SkySphere::~SkySphere()
{
	if (m_quadric != nullptr)
	{
		gluDeleteQuadric(static_cast<GLUquadric*>(m_quadric));
	}
}

void SkySphere::Render(const TextureLibrary& textures, const Vector3& cameraPosition) const
{
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT);

	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_FALSE);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	textures.Bind(TextureKey::Sky);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	gluSphere(
		static_cast<GLUquadric*>(m_quadric),
		RenderConfig::SKY_RADIUS,
		RenderConfig::SKY_SLICES,
		RenderConfig::SKY_STACKS);

	glPopMatrix();
	glPopAttrib();
}
