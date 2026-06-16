#include "OpenGLRenderer.h"
#include "mash/Mesh.h"
#include "shader/Shader.h"
#include <glad/glad.h>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtc/type_ptr.hpp>

namespace
{
// TODO спрятать нахуй!!!!
glm::mat4 CalculateProjectionMatrix(float fov, float aspect)
{
	constexpr float nearPlane = 0.1f;
	constexpr float farPlane = 100.0f;

	return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

glm::mat4 CalculateViewMatrix(const CameraState& camera)
{
	float yawRad = glm::radians(camera.yaw);
	float pitchRad = glm::radians(camera.pitch);

	glm::vec3 front;
	front.x = std::cos(yawRad) * std::cos(pitchRad);
	front.y = std::sin(pitchRad);
	front.z = std::sin(yawRad) * std::cos(pitchRad);
	front = glm::normalize(front);

	glm::vec3 upWorld(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(front, upWorld));
	glm::vec3 up = glm::normalize(glm::cross(right, front));
	glm::vec3 position(camera.position.x, camera.position.y, camera.position.z);

	return glm::lookAt(position, position + front, up);
}

glm::mat4 CalculateModelMatrix(const SceneObject& object)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(object.position.x, object.position.y, object.position.z));
	model = glm::rotate(model, glm::radians(object.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(object.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(object.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(object.scale.x, object.scale.y, object.scale.z));
	return model;
}

struct Point3D
{
	float x, y, z;
};
} // namespace

OpenGLRenderer::OpenGLRenderer()
{
	InitShaders();
	InitGeometry();
}

OpenGLRenderer::~OpenGLRenderer()
{
}

void OpenGLRenderer::SetViewport(uint32_t width, uint32_t height)
{
	m_viewportWidth = width;
	m_viewportHeight = height;
}

void OpenGLRenderer::SetClearColor(const Color& color)
{
	m_clearColor = color;
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

void OpenGLRenderer::RenderFrame(const RenderData& data)
{
	if (m_viewportHeight == 0 || m_viewportWidth == 0)
	{
		return;
	}

	glEnable(GL_DEPTH_TEST);
	// тут рисование, хотя хз нужно ли тут тест глубины делать
	glDisable(GL_DEPTH_TEST);
}

void OpenGLRenderer::InitShaders()
{
	m_shaders["main"] = std::make_unique<Shader>("static/shaders/main.vert", "static/shaders/main.frag");
}

void OpenGLRenderer::InitGeometry()
{
}