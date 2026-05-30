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
constexpr std::array<float, CANVAS_VERTICES_SIZE> CANVAS_VERTICES = {
	-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f};

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

void GenerateCylinder(std::vector<float>& vertices, int segments, float radius, float height)
{
	float halfHeight = height / 2.0f;
	float angleStep = 2.0f * std::numbers::pi_v<float> / segments;

	for (int i = 0; i < segments; ++i)
	{
		float angle1 = i * angleStep;
		float angle2 = (i + 1) * angleStep;

		float x1 = radius * std::cos(angle1);
		float z1 = radius * std::sin(angle1);
		float x2 = radius * std::cos(angle2);
		float z2 = radius * std::sin(angle2);

		vertices.insert(vertices.end(), {x1, -halfHeight, z1, x2, -halfHeight, z2, 0.0f, -halfHeight, 0.0f});
		vertices.insert(vertices.end(), {x1, halfHeight, z1, 0.0f, halfHeight, 0.0f, x2, halfHeight, z2});
		vertices.insert(vertices.end(), {x1, -halfHeight, z1, x1, halfHeight, z1, x2, -halfHeight, z2});
		vertices.insert(vertices.end(), {x2, -halfHeight, z2, x1, halfHeight, z1, x2, halfHeight, z2});
	}
}

void GenerateTorus(std::vector<float>& vertices, int mainSegments, int tubeSegments, float mainRadius, float tubeRadius)
{
	float mainStep = 2.0f * std::numbers::pi_v<float> / mainSegments;
	float tubeStep = 2.0f * std::numbers::pi_v<float> / tubeSegments;

	for (int i = 0; i < mainSegments; ++i)
	{
		float mainAngle1 = i * mainStep;
		float mainAngle2 = (i + 1) * mainStep;

		for (int j = 0; j < tubeSegments; ++j)
		{
			float tubeAngle1 = j * tubeStep;
			float tubeAngle2 = (j + 1) * tubeStep;

			auto getVertex = [=](float mAngle, float tAngle) {
				float x = (mainRadius + tubeRadius * std::cos(tAngle)) * std::cos(mAngle);
				float y = tubeRadius * std::sin(tAngle);
				float z = (mainRadius + tubeRadius * std::cos(tAngle)) * std::sin(mAngle);
				return std::array<float, 3>{x, y, z};
			};

			auto v1 = getVertex(mainAngle1, tubeAngle1);
			auto v2 = getVertex(mainAngle2, tubeAngle1);
			auto v3 = getVertex(mainAngle1, tubeAngle2);
			auto v4 = getVertex(mainAngle2, tubeAngle2);

			vertices.insert(vertices.end(), {v1[0], v1[1], v1[2], v2[0], v2[1], v2[2], v3[0], v3[1], v3[2]});
			vertices.insert(vertices.end(), {v3[0], v3[1], v3[2], v2[0], v2[1], v2[2], v4[0], v4[1], v4[2]});
		}
	}
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

void OpenGLRenderer::RenderFrame(const ModelData& data)
{
	if (m_viewportHeight == 0 || m_viewportWidth == 0)
	{
		return;
	}

	glEnable(GL_DEPTH_TEST);

	RenderMainPass(data);
	RenderGlowMaskPass(data);

	glDisable(GL_DEPTH_TEST);

	ApplyBlurPass();
	RenderCompositePass();
}

void OpenGLRenderer::RenderMainPass(const ModelData& data)
{
	m_mainFbo->Bind();
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	auto [r, g, b, a] = m_clearColor.GetAsFloats();

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto& shader = m_shaders["main"];
	shader->Use();

	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	glm::mat4 projection = glm::perspective(glm::radians(data.camera.fov), aspect, 0.1f, 100.0f);
	glm::mat4 view = CalculateViewMatrix(data.camera);

	shader->SetMat4("u_projection", glm::value_ptr(projection));
	shader->SetMat4("u_view", glm::value_ptr(view));

	for (const auto& object : data.objects)
	{
		shader->SetFloat4("u_color", object.color.GetAsFloats());
		DrawObject(object, shader);
	}
}

void OpenGLRenderer::RenderGlowMaskPass(const ModelData& data)
{
	m_glowFbo->Bind();
	glViewport(0, 0, m_viewportWidth / 2, m_viewportHeight / 2);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const auto& shader = m_shaders["mask"];
	shader->Use();

	float aspect = static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight);
	glm::mat4 projection = glm::perspective(glm::radians(data.camera.fov), aspect, 0.1f, 100.0f);
	glm::mat4 view = CalculateViewMatrix(data.camera);

	shader->SetMat4("u_projection", glm::value_ptr(projection));
	shader->SetMat4("u_view", glm::value_ptr(view));

	for (const auto& object : data.objects)
	{
		if (object.isEmissive)
		{
			shader->SetFloat4("u_color", object.color.GetAsFloats());
		}
		else
		{
			shader->SetFloat4("u_color", {0.0f, 0.0f, 0.0f, 1.0f});
		}

		DrawObject(object, shader);
	}
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

		m_canvasMesh->Bind();
		glDrawArrays(GL_TRIANGLES, 0, m_canvasMesh->GetVertexCount());

		horizontal = !horizontal;
		if (firstIteration)
		{
			firstIteration = false;
		}
	}
}

void OpenGLRenderer::RenderCompositePass()
{
	Framebuffer::Unbind();
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	const auto& shader = m_shaders["composite"];
	shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainFbo->GetColorTexture());
	shader->SetInt("u_mainTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pingPongFbos[0]->GetColorTexture());
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

void OpenGLRenderer::InitShaders()
{
	m_shaders["main"] = std::make_unique<Shader>("static/shaders/main.vert", "static/shaders/main.frag");
	m_shaders["mask"] = std::make_unique<Shader>("static/shaders/main.vert", "static/shaders/mask.frag");
	m_shaders["blur"] = std::make_unique<Shader>("static/shaders/canvas.vert", "static/shaders/blur.frag");
	m_shaders["composite"] = std::make_unique<Shader>("static/shaders/canvas.vert", "static/shaders/composite.frag");
}

void OpenGLRenderer::InitGeometry()
{
	std::vector<float> canvasVertices(CANVAS_VERTICES.begin(), CANVAS_VERTICES.end());
	m_canvasMesh = std::make_unique<Mesh>(canvasVertices, 2);

	std::vector<float> cylinderVertices;
	GenerateCylinder(cylinderVertices, 32, 0.5f, 2.0f);
	m_cylinderMesh = std::make_unique<Mesh>(cylinderVertices, 3);

	std::vector<float> torusVertices;
	GenerateTorus(torusVertices, 48, 24, 1.5f, 0.4f);
	m_torusMesh = std::make_unique<Mesh>(torusVertices, 3);
}

void OpenGLRenderer::InitFramebuffers()
{
	FramebufferConfig mainConfig;
	mainConfig.width = m_viewportWidth;
	mainConfig.height = m_viewportHeight;
	mainConfig.hasDepthBuffer = true;
	mainConfig.useClampToEdge = false;

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
}