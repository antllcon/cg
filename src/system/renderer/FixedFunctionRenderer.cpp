#include "FixedFunctionRenderer.h"
#include "src/system/AppConfig.h"
#include "src/system/renderer/gl/GlHeaders.h"
#include <cmath>
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

void EnableGlobalRenderStates()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	const GLfloat globalAmbient[4] = {0.04f, 0.04f, 0.04f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	const auto [fr, fg, fb, fa] = RenderConfig::FOG_COLOR.GetAsFloats();
	const GLfloat fogColor[4] = {fr, fg, fb, fa};
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, RenderConfig::FOG_DENSITY);
}

void SetupProjection(float fov, uint32_t width, uint32_t height)
{
	const double aspect = static_cast<double>(width) / static_cast<double>(height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, NEAR_PLANE, FAR_PLANE);
}

void ApplyCameraState(const CameraState& camera)
{
	const Vec3 front = CameraFront(camera);
	gluLookAt(
		camera.position.x,
		camera.position.y,
		camera.position.z,
		camera.position.x + front.x,
		camera.position.y + front.y,
		camera.position.z + front.z,
		0.0,
		1.0,
		0.0);
}

void ApplyLightState(const Light& light)
{
	const GLfloat position[4] = {light.position.x, light.position.y, light.position.z, 1.0f};
	const auto [r, g, b, a] = light.color.GetAsFloats();
	const GLfloat diffuse[4] = {r, g, b, a};
	const GLfloat ambient[4] = {
		r * light.ambientStrength,
		g * light.ambientStrength,
		b * light.ambientStrength,
		1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light.constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light.linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light.quadratic);
}

void SetupModelView(const CameraState& camera, const Light& light)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ApplyCameraState(camera);
	ApplyLightState(light);
}

void ApplyTransform(const SceneObject& object)
{
	glTranslatef(object.position.x, object.position.y, object.position.z);
	glRotatef(object.rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(object.rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(object.rotation.z, 0.0f, 0.0f, 1.0f);
	glScalef(object.scale.x, object.scale.y, object.scale.z);
}

void ApplyMaterial(const SceneObject& object)
{
	const auto [r, g, b, a] = object.color.GetAsFloats();
	glColor4f(r, g, b, a);

	const GLfloat specular[4] = {
		object.specularStrength,
		object.specularStrength,
		object.specularStrength,
		1.0f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, object.shininess);
}
} // namespace

FixedFunctionRenderer::FixedFunctionRenderer()
{
	m_shapes[RenderableShape::Cube] = Geometry::BuildUnitCube();
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

	EnableGlobalRenderStates();
	SetupProjection(data.camera.fov, m_viewportWidth, m_viewportHeight);
	SetupModelView(data.camera, data.light);

	m_sky.Render(m_textures, data.camera.position);

	for (const auto& object : data.objects)
	{
		DrawObject(object);
	}
}

void FixedFunctionRenderer::DrawObject(const SceneObject& object) const
{
	const auto it = m_shapes.find(object.shape);
	if (it == m_shapes.end())
	{
		return;
	}

	const bool useShadow = object.useShadow && m_multiTexture.IsSupported();

	glPushMatrix();

	ApplyTransform(object);
	ApplyMaterial(object);

	BindBaseTexture(object.texture);
	if (useShadow)
	{
		BindShadowTexture();
	}

	DrawGeometry(it->second, object.uvScaleU, object.uvScaleV, useShadow);

	if (useShadow)
	{
		UnbindShadowTexture();
	}

	glPopMatrix();
}

void FixedFunctionRenderer::DrawGeometry(const std::vector<Geometry::Vertex>& vertices, float uvScaleU, float uvScaleV, bool useShadow) const
{
	glBegin(GL_TRIANGLES);
	for (const auto& vertex : vertices)
	{
		glNormal3f(vertex.norX, vertex.norY, vertex.norZ);

		if (useShadow)
		{
			m_multiTexture.Coord(0, vertex.texU * uvScaleU, vertex.texV * uvScaleV);
			m_multiTexture.Coord(1, vertex.texU, vertex.texV);
		}
		else
		{
			glTexCoord2f(vertex.texU * uvScaleU, vertex.texV * uvScaleV);
		}

		glVertex3f(vertex.posX, vertex.posY, vertex.posZ);
	}
	glEnd();
}

void FixedFunctionRenderer::BindBaseTexture(TextureKey texture) const
{
	m_multiTexture.ActiveTexture(0);
	glEnable(GL_TEXTURE_2D);
	m_textures.Bind(texture);
}

void FixedFunctionRenderer::BindShadowTexture() const
{
	m_multiTexture.ActiveTexture(1);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	m_textures.Bind(TextureKey::Shadow);
}

void FixedFunctionRenderer::UnbindShadowTexture() const
{
	m_multiTexture.ActiveTexture(1);
	glDisable(GL_TEXTURE_2D);
	m_multiTexture.ActiveTexture(0);
}
