#include "OpenGLRenderer.h"
#include <array>
#include <glad/glad.h>
#include <libs/glm/glm.hpp>
#include <libs/glm/gtc/matrix_transform.hpp>
#include <libs/glm/gtc/type_ptr.hpp>
#include <numbers>
#include <stdexcept>

namespace
{
constexpr uint8_t CANVAS_VERTICES_SIZE = 12u;
constexpr std::array<float, CANVAS_VERTICES_SIZE> CANVAS_VERTICES = {
	-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f};

void AssertIsFramebufferComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error("Ошибка инициализации Framebuffer: буфер не собран");
	}
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
	CleanupResources();
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_mainFbo);
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
	glBindFramebuffer(GL_FRAMEBUFFER, m_glowFbo);
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
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFbo[horizontal]);
		shader->SetInt("u_horizontal", horizontal);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, firstIteration ? m_glowColorTex : m_pingPongTex[!horizontal]);

		glBindVertexArray(m_canvasVao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		horizontal = !horizontal;
		if (firstIteration)
		{
			firstIteration = false;
		}
	}
}

void OpenGLRenderer::RenderCompositePass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_viewportWidth, m_viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT);

	const auto& shader = m_shaders["composite"];
	shader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mainColorTex);
	shader->SetInt("u_mainTexture", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_pingPongTex[0]);
	shader->SetInt("u_glowTexture", 1);

	glBindVertexArray(m_canvasVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::DrawObject(const SceneObject& object, const std::unique_ptr<Shader>& shader)
{
	glm::mat4 model = CalculateModelMatrix(object);
	shader->SetMat4("u_model", glm::value_ptr(model));

	if (object.type == ObjectType::Cylinder)
	{
		glBindVertexArray(m_cylinderVao);
		glDrawArrays(GL_TRIANGLES, 0, m_cylinderVertexCount);
	}
	else if (object.type == ObjectType::Torus)
	{
		glBindVertexArray(m_torusVao);
		glDrawArrays(GL_TRIANGLES, 0, m_torusVertexCount);
	}

	glBindVertexArray(0);
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
	glGenVertexArrays(1, &m_canvasVao);
	glGenBuffers(1, &m_canvasVbo);
	glBindVertexArray(m_canvasVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_canvasVbo);
	glBufferData(GL_ARRAY_BUFFER, CANVAS_VERTICES_SIZE * sizeof(float), CANVAS_VERTICES.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	std::vector<float> cylinderVertices;
	GenerateCylinder(cylinderVertices, 32, 0.5f, 2.0f);
	m_cylinderVertexCount = static_cast<uint32_t>(cylinderVertices.size() / 3);

	glGenVertexArrays(1, &m_cylinderVao);
	glGenBuffers(1, &m_cylinderVbo);
	glBindVertexArray(m_cylinderVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_cylinderVbo);
	glBufferData(GL_ARRAY_BUFFER, cylinderVertices.size() * sizeof(float), cylinderVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	std::vector<float> torusVertices;
	GenerateTorus(torusVertices, 48, 24, 1.5f, 0.4f);
	m_torusVertexCount = static_cast<uint32_t>(torusVertices.size() / 3);

	glGenVertexArrays(1, &m_torusVao);
	glGenBuffers(1, &m_torusVbo);
	glBindVertexArray(m_torusVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_torusVbo);
	glBufferData(GL_ARRAY_BUFFER, torusVertices.size() * sizeof(float), torusVertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void OpenGLRenderer::InitFramebuffers()
{
	if (m_mainFbo != 0)
	{
		CleanupResources();
	}

	glGenFramebuffers(1, &m_mainFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_mainFbo);

	glGenTextures(1, &m_mainColorTex);
	glBindTexture(GL_TEXTURE_2D, m_mainColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_viewportWidth, m_viewportHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_mainColorTex, 0);

	glGenRenderbuffers(1, &m_mainDepthRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_mainDepthRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_viewportWidth, m_viewportHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_mainDepthRbo);
	AssertIsFramebufferComplete();

	glGenFramebuffers(1, &m_glowFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_glowFbo);

	glGenTextures(1, &m_glowColorTex);
	glBindTexture(GL_TEXTURE_2D, m_glowColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_viewportWidth / 2, m_viewportHeight / 2, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glowColorTex, 0);

	glGenRenderbuffers(1, &m_glowDepthRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_glowDepthRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_viewportWidth / 2, m_viewportHeight / 2);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_glowDepthRbo);
	AssertIsFramebufferComplete();

	glGenFramebuffers(2, m_pingPongFbo);
	glGenTextures(2, m_pingPongTex);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFbo[i]);
		glBindTexture(GL_TEXTURE_2D, m_pingPongTex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_viewportWidth / 2, m_viewportHeight / 2, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingPongTex[i], 0);
		AssertIsFramebufferComplete();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderer::CleanupResources()
{
	glDeleteFramebuffers(1, &m_mainFbo);
	glDeleteTextures(1, &m_mainColorTex);
	glDeleteRenderbuffers(1, &m_mainDepthRbo);

	glDeleteFramebuffers(1, &m_glowFbo);
	glDeleteTextures(1, &m_glowColorTex);
	glDeleteRenderbuffers(1, &m_glowDepthRbo);

	glDeleteFramebuffers(2, m_pingPongFbo);
	glDeleteTextures(2, m_pingPongTex);
}