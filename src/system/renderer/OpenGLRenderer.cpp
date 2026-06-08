#include "OpenGLRenderer.h"
#include <array>
#include <glad/glad.h>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <numbers>

namespace
{
constexpr uint8_t CANVAS_VERTICES_SIZE = 12u;
constexpr std::array<float, CANVAS_VERTICES_SIZE> CANVAS_VERTICES = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f};

constexpr int SEGMENT_TRIANGLES = 4;
constexpr int TRIANGLE_VERTICES = 3;
constexpr int VERTEX_CORD = 3;

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

Point3D GetPointOnCircle(float radius, float angleRadians)
{
	return {
		radius * std::cos(angleRadians),
		0.0f,
		radius * std::sin(angleRadians)};
}

Point3D GetCircleCenter(float yLevel)
{
	return {0.0f, yLevel, 0.0f};
}

Point3D GetRimPoint(float radius, float angleRadians, float yLevel)
{
	auto circlePoint = GetPointOnCircle(radius, angleRadians);
	return {circlePoint.x, yLevel, circlePoint.z};
}

void AddTriangle(std::vector<float>& vertices, const Point3D& a, const Point3D& b, const Point3D& c)
{
	vertices.insert(vertices.end(), {a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z});
}

std::vector<float> GenerateCylinder(int segments, float radius, float height)
{
	std::vector<float> vertices;
	vertices.reserve(segments * SEGMENT_TRIANGLES * TRIANGLE_VERTICES * VERTEX_CORD);

	const float topY = height / 2.0f;
	const float bottomY = -height / 2.0f;
	const float angleStep = 2.0f * std::numbers::pi_v<float> / segments;

	for (int i = 0; i < segments; ++i)
	{
		float currentAngle = i * angleStep;
		float nextAngle = (i + 1) * angleStep;

		Point3D bottomCurrent = GetRimPoint(radius, currentAngle, bottomY);
		Point3D bottomNext = GetRimPoint(radius, nextAngle, bottomY);
		Point3D topCurrent = GetRimPoint(radius, currentAngle, topY);
		Point3D topNext = GetRimPoint(radius, nextAngle, topY);

		Point3D centerBottom = GetCircleCenter(bottomY);
		Point3D centerTop = GetCircleCenter(topY);

		AddTriangle(vertices, bottomCurrent, bottomNext, centerBottom);
		AddTriangle(vertices, topCurrent, centerTop, topNext);
		AddTriangle(vertices, bottomCurrent, topCurrent, bottomNext);
		AddTriangle(vertices, bottomNext, topCurrent, topNext);
	}

	return vertices;
}

std::vector<float> GenerateTorus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius)
{
	std::vector<float> vertices;
	vertices.reserve(mainSegments * tubeSegments * 18);

	const float mainStep = 2.0f * std::numbers::pi_v<float> / mainSegments;
	const float tubeStep = 2.0f * std::numbers::pi_v<float> / tubeSegments;

	auto getVertex = [=](float mAngle, float tAngle) {
		float distanceToCenter = mainRadius + tubeRadius * std::cos(tAngle);

		float x = distanceToCenter * std::cos(mAngle);
		float y = tubeRadius * std::sin(tAngle);
		float z = distanceToCenter * std::sin(mAngle);

		return std::array<float, 3>{x, y, z};
	};

	for (int i = 0; i < mainSegments; ++i)
	{
		float mainAngle1 = i * mainStep;
		float mainAngle2 = (i + 1) * mainStep;

		for (int j = 0; j < tubeSegments; ++j)
		{
			float tubeAngle1 = j * tubeStep;
			float tubeAngle2 = (j + 1) * tubeStep;

			auto v1 = getVertex(mainAngle1, tubeAngle1);
			auto v2 = getVertex(mainAngle2, tubeAngle1);
			auto v3 = getVertex(mainAngle1, tubeAngle2);
			auto v4 = getVertex(mainAngle2, tubeAngle2);

			vertices.insert(vertices.end(), {v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]});
			vertices.insert(vertices.end(), {v3[0], v3[1], v3[2], v2[0], v2[1], v2[2], v4[0], v4[1], v4[2]});
		}
	}

	return vertices;
}
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

	InitFramebuffers();
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
	RenderGeometryPass(data);
	glDisable(GL_DEPTH_TEST);

	DownscaleGlowPass();
	ApplyBlurPass();
	ApplyLightMotionBlurPass();
	RenderCompositePass();
}

void OpenGLRenderer::InitShaders()
{
	m_shaders["main"] = std::make_unique<Shader>("static/shaders/main.vert", "static/shaders/main.frag");
	m_shaders["blur"] = std::make_unique<Shader>("static/shaders/canvas.vert", "static/shaders/blur.frag");
	m_shaders["composite"] = std::make_unique<Shader>("static/shaders/canvas.vert", "static/shaders/composite.frag");
	m_shaders["copy"] = std::make_unique<Shader>("static/shaders/canvas.vert", "static/shaders/copy.frag");
	m_shaders["motion_blur"] = std::make_unique<Shader>("static/shaders/canvas.vert", "static/shaders/motion_blur.frag");
}

void OpenGLRenderer::InitGeometry()
{
	std::vector<float> canvasVertices(CANVAS_VERTICES.begin(), CANVAS_VERTICES.end());
	m_canvasMesh = std::make_unique<Mesh>(canvasVertices, uint8_t{2});

	std::vector<float> cylinderVertices = GenerateCylinder(32, 0.5f, 2.0f);
	m_cylinderMesh = std::make_unique<Mesh>(cylinderVertices, uint8_t{3});

	std::vector<float> torusVertices = GenerateTorus(48, 24, 1.5f, 0.4f);
	m_torusMesh = std::make_unique<Mesh>(torusVertices, uint8_t{3});
}

void OpenGLRenderer::InitFramebuffers()
{
	FramebufferConfig mainConfig;
	mainConfig.width = m_viewportWidth;
	mainConfig.height = m_viewportHeight;
	mainConfig.hasDepthBuffer = true;
	mainConfig.useClampToEdge = false;
	mainConfig.colorAttachmentCount = 2;

	m_mainFbo = std::make_unique<Framebuffer>(mainConfig);

	FramebufferConfig glowConfig;
	glowConfig.width = m_viewportWidth / 2;
	glowConfig.height = m_viewportHeight / 2;
	glowConfig.hasDepthBuffer = true;
	glowConfig.useClampToEdge = true;

	m_glowFbo = std::make_unique<Framebuffer>(glowConfig);

	FramebufferConfig pingPongConfig;
	pingPongConfig.width = m_viewportWidth / 2;
	pingPongConfig.height = m_viewportHeight / 2;
	pingPongConfig.hasDepthBuffer = false;
	pingPongConfig.useClampToEdge = true;

	m_pingPongFbos[0] = std::make_unique<Framebuffer>(pingPongConfig);
	m_pingPongFbos[1] = std::make_unique<Framebuffer>(pingPongConfig);

	FramebufferConfig historyConfig;
	historyConfig.width = m_viewportWidth / 2;
	historyConfig.height = m_viewportHeight / 2;
	historyConfig.hasDepthBuffer = false;
	historyConfig.useClampToEdge = true;

	m_historyFbos[0] = std::make_unique<Framebuffer>(historyConfig);
	m_historyFbos[1] = std::make_unique<Framebuffer>(historyConfig);
}

void OpenGLRenderer::ClearGeometryBuffers()
{
	auto [r, g, b, a] = m_clearColor.GetAsFloats();
	const std::array<float, 4> bgColor = {r, g, b, a};
	const std::array<float, 4> blackColor = {0.0f, 0.0f, 0.0f, 1.0f};

	glClearBufferfv(GL_COLOR, 0, bgColor.data());
	glClearBufferfv(GL_COLOR, 1, blackColor.data());

	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::SetupShaderUniforms(const Shader& shader, const CameraState& camera)
{
	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);

	glm::mat4 projection = CalculateProjectionMatrix(camera.fov, aspect);
	glm::mat4 view = CalculateViewMatrix(camera);

	shader.SetMat4("u_projection", glm::value_ptr(projection));
	shader.SetMat4("u_view", glm::value_ptr(view));
}

void OpenGLRenderer::RenderSceneObjects(const std::vector<SceneObject>& objects, const std::unique_ptr<Shader>& shader)
{
	for (const auto& object : objects)
	{
		shader->SetFloat4("u_color", object.color.GetAsFloats());
		shader->SetBool("u_isEmissive", object.isEmissive);
		DrawObject(object, shader);
	}
}

void OpenGLRenderer::RenderGeometryPass(const RenderData& data)
{
	m_mainFbo->Bind();
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);

	ClearGeometryBuffers();

	const auto& shader = m_shaders["main"];
	shader->Use();

	SetupShaderUniforms(*shader, data.camera);
	RenderSceneObjects(data.objects, shader);
}

void OpenGLRenderer::DownscaleGlowPass()
{
	m_glowFbo->Bind();
	glViewport(0, 0, m_viewportWidth / 2, m_viewportHeight / 2);
	glClear(GL_COLOR_BUFFER_BIT);

	const auto& shader = m_shaders["copy"];
	shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainFbo->GetColorTexture(1));
	shader->SetInt("u_texture", 0);

	m_canvasMesh->Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_canvasMesh->GetVertexCount());
}

void OpenGLRenderer::ApplyLightMotionBlurPass()
{
	// Определяем, в какой буфер будем писать (противоположный от текущего)
	uint8_t writeIdx = 1 - m_currentHistoryIdx;

	m_historyFbos[writeIdx]->Bind();
	glViewport(0, 0, m_viewportWidth / 2, m_viewportHeight / 2);
	glClear(GL_COLOR_BUFFER_BIT);

	const auto& shader = m_shaders["motion_blur"];
	shader->Use();

	// 1. Текущее размытое свечение (результат Пинг-Понга)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pingPongFbos[0]->GetColorTexture());
	shader->SetInt("u_currentGlow", 0);

	// 2. История предыдущего кадра
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_historyFbos[m_currentHistoryIdx]->GetColorTexture());
	shader->SetInt("u_historyGlow", 1);

	// Настраиваем длину "хвоста" (от 0.0 до 1.0)
	shader->SetFloat("u_fadeFactor", 0.9f);

	m_canvasMesh->Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_canvasMesh->GetVertexCount());

	// Сохраняем индекс для следующего кадра
	m_currentHistoryIdx = writeIdx;
}

void OpenGLRenderer::ApplyBlurPass()
{
	bool horizontal = true;
	bool firstIteration = true;
	uint8_t amount = 10;

	const auto& shader = m_shaders["blur"];
	shader->Use();
	glViewport(0, 0, m_viewportWidth / 2, m_viewportHeight / 2);

	for (uint8_t i = 0; i < amount; i++)
	{
		m_pingPongFbos[horizontal]->Bind();
		shader->SetInt("u_horizontal", horizontal);

		glActiveTexture(GL_TEXTURE0);
		uint32_t textureId = firstIteration ? m_glowFbo->GetColorTexture() : m_pingPongFbos[!horizontal]->GetColorTexture();
		glBindTexture(GL_TEXTURE_2D, textureId);
		shader->SetInt("u_image", 0);

		m_canvasMesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_canvasMesh->GetVertexCount());

		horizontal = !horizontal;
		firstIteration = false;
	}
}

void OpenGLRenderer::RenderCompositePass()
{
	Framebuffer::Unbind(); // Рисуем прямо на экран монитора
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto& shader = m_shaders["composite"];
	shader->Use();

	// 1. Основная четкая сцена (Слой 0)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainFbo->GetColorTexture(0));
	shader->SetInt("u_mainTexture", 0);

	// 2. Финальное свечение с инерционным следом
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_historyFbos[m_currentHistoryIdx]->GetColorTexture());
	shader->SetInt("u_glowTexture", 1);

	m_canvasMesh->Bind();
	glDrawArrays(GL_TRIANGLES, 0, m_canvasMesh->GetVertexCount());
}

void OpenGLRenderer::DrawObject(const SceneObject& object, const std::unique_ptr<Shader>& shader)
{
	glm::mat4 model = CalculateModelMatrix(object);
	shader->SetMat4("u_model", glm::value_ptr(model));

	if (object.type == ObjectType::Cylinder)
	{
		m_cylinderMesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_cylinderMesh->GetVertexCount());
	}
	else if (object.type == ObjectType::Torus)
	{
		m_torusMesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_torusMesh->GetVertexCount());
	}

	Mesh::Unbind();
}