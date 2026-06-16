#include "FixedFunctionRenderer.h"
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <numbers>

namespace
{
constexpr double NEAR_PLANE = 0.1;
constexpr double FAR_PLANE = 100.0;
constexpr float DEG_TO_RAD = static_cast<float>(std::numbers::pi) / 180.0f;

struct Vec3
{
	float x;
	float y;
	float z;
};

Vec3 CameraFront(const CameraState& camera)
{
	const float yaw = camera.yaw * DEG_TO_RAD;
	const float pitch = camera.pitch * DEG_TO_RAD;

	return {
		std::cos(yaw) * std::cos(pitch),
		std::sin(pitch),
		std::sin(yaw) * std::cos(pitch)};
}
} // namespace

FixedFunctionRenderer::FixedFunctionRenderer()
{
	m_shapes[RenderableShape::MobiusStrip] = Geometry::BuildMobiusStrip(30, 20, 2.0f, 0.6f);
}

void FixedFunctionRenderer::SetViewport(uint32_t width, uint32_t height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void FixedFunctionRenderer::SetClearColor(const Color& color)
{
	m_clearColor = color;
	const auto [r, g, b, a] = color.GetAsFloats();
	glClearColor(r, g, b, a);
}

void FixedFunctionRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FixedFunctionRenderer::Display()
{
	glFlush();
}

void FixedFunctionRenderer::RenderFrame(const RenderData& data)
{
	if (m_viewportWidth == 0 || m_viewportHeight == 0)
	{
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	const double aspect = static_cast<double>(m_viewportWidth) / static_cast<double>(m_viewportHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(data.camera.fov, aspect, NEAR_PLANE, FAR_PLANE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ApplyCamera(data.camera);

	ApplyLight(data.light);

	for (const auto& object : data.objects)
	{
		DrawObject(object);
	}
}

void FixedFunctionRenderer::ApplyCamera(const CameraState& camera) const
{
	const Vec3 front = CameraFront(camera);
	const Vec3 eye = {camera.position.x, camera.position.y, camera.position.z};

	gluLookAt(
		eye.x, eye.y, eye.z, eye.x + front.x, eye.y + front.y, eye.z + front.z, 0.0, 1.0, 0.0);
}

void FixedFunctionRenderer::ApplyLight(const Light& light) const
{
	const GLfloat position[4] = {light.position.x, light.position.y, light.position.z, 1.0f};
	const auto [r, g, b, a] = light.color.GetAsFloats();
	const GLfloat diffuse[4] = {r, g, b, a};

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
}

void FixedFunctionRenderer::DrawObject(const SceneObject& object) const
{
	const auto it = m_shapes.find(object.shape);
	if (it == m_shapes.end())
	{
		return;
	}

	glPushMatrix();

	glTranslatef(object.position.x, object.position.y, object.position.z);
	glRotatef(object.rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(object.rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(object.rotation.z, 0.0f, 0.0f, 1.0f);
	glScalef(object.scale.x, object.scale.y, object.scale.z);

	const auto [r, g, b, a] = object.color.GetAsFloats();
	glColor4f(r, g, b, a);

	const GLfloat specular[4] = {
		object.specularStrength,
		object.specularStrength,
		object.specularStrength,
		1.0f};
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, object.shininess);

	glBegin(GL_TRIANGLES);
	for (const auto& [px, py, pz, nx, ny, nz] : it->second)
	{
		glNormal3f(nx, ny, nz);
		glVertex3f(px, py, pz);
	}
	glEnd();

	glPopMatrix();
}