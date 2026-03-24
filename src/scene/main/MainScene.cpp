#include "MainScene.h"
#include "src/controller/cube/CubeController.h"
#include "src/controller/theme/ThemeController.h"
#include "src/core/utils/FileReader.h"
#include "src/system/AppConfig.h"
#include "src/system/render/opengl/OpenglGeometryFactory.h"
#include "src/system/render/opengl/OpenglMaterial.h"
#include "src/system/render/opengl/OpenglShader.h"
#include <libs/glm/gtc/matrix_transform.hpp>

void MainScene::Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager&)
{
	m_camera.position = {0.0f, 2.0f, 6.0f};
	m_camera.viewMatrix = glm::lookAt(m_camera.position, Point3f{0.0f, 0.0f, 0.0f}, Point3f{0.0f, 1.0f, 0.0f});
	m_camera.projectionMatrix = glm::perspective(glm::radians(45.0f), AppConfig::WINDOW_WIDTH / AppConfig::WINDOW_HEIGHT, 0.1f, 100.0f);

	m_light.position = {3.0f, 5.0f, 4.0f};
	m_light.color = Color::FromFloat(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.intensity = 1.0f;

	auto themeController = std::make_shared<ThemeController>(themeModel);
	AddController(themeController);

	m_themeView = std::make_shared<ThemeView>(themeModel, themeController);
	themeModel->RegisterObserver(m_themeView);
	AddView(m_themeView);

	auto toastModel = std::make_shared<ToastModel>();
	AddModel(toastModel);

	m_toastController = std::make_shared<ToastController>(toastModel);
	AddController(m_toastController);

	m_toastView = std::make_shared<ToastView>(toastModel, themeModel);
	toastModel->RegisterObserver(m_toastView);
	themeModel->RegisterObserver(m_toastView);
	AddView(m_toastView);

	OpenglGeometryFactory geometryFactory;
	auto cubeMesh = geometryFactory.CreateCube(2.0f);

	std::string vertSource = FileReader::ReadFileToString("static/shaders/phong.vert");
	std::string fragSource = FileReader::ReadFileToString("static/shaders/phong.frag");
	auto cubeShader = std::make_shared<OpenglShader>(vertSource, fragSource);

	auto cubeMaterial = std::make_shared<OpenglMaterial>();
	cubeMaterial->SetShader(cubeShader);

	auto cubeModel = std::make_shared<CubeModel>();
	AddModel(cubeModel);

	auto cubeController = std::make_shared<CubeController>(cubeModel);
	AddController(cubeController);

	m_cubeView = std::make_shared<CubeView>(cubeModel, cubeController, cubeMesh, cubeMaterial);
	cubeModel->RegisterObserver(m_cubeView);
	AddView(m_cubeView);
}

void MainScene::Render(IRenderer& renderer) const
{
	renderer.BeginFrame(m_camera);
	renderer.SubmitLight(m_light);
	m_cubeView->Render(renderer);
	renderer.EndFrame();

	renderer.BeginUI();
	m_themeView->Render(renderer);
	m_toastView->Render(renderer);
	renderer.EndUI();
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}