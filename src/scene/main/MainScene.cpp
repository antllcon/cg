#include "MainScene.h"
#include "src/controller/camera/CameraController.h"
#include "src/controller/entity/EntityController.h"
#include "src/controller/sun/SunController.h"
#include "src/controller/theme/ThemeController.h"
#include "src/controller/window/WindowController.h"
#include "src/core/utils/FileReader.h"
#include "src/system/AppConfig.h"
#include "src/system/render/opengl/OpenglGeometryFactory.h"
#include "src/system/render/opengl/OpenglMaterial.h"
#include "src/system/render/opengl/OpenglShader.h"
#include "src/view/camera/CameraView.h"
#include "src/view/theme/ThemeView.h"
#include "src/view/toast/ToastView.h"

void MainScene::Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager&, IWindow& window)
{
	auto windowController = std::make_shared<WindowController>(window);
	m_sceneController->AddController(windowController);

	m_cameraModel = std::make_shared<CameraModel>();
	m_cameraModel->Init({0.0f, 2.0f, 6.0f}, 45.0f, AppConfig::WINDOW_WIDTH / AppConfig::WINDOW_HEIGHT, 0.1f, 100.0f);
	m_sceneView->SetCamera(m_cameraModel);

	auto cameraController = std::make_shared<CameraController>(m_cameraModel, window);
	m_sceneController->AddController(cameraController);

	auto cameraView = std::make_shared<CameraView>(m_cameraModel, cameraController);
	m_cameraModel->RegisterObserver(cameraView);
	m_sceneView->AddUiView(cameraView);

	auto sunModel = std::make_shared<SunModel>();
	m_sceneModel->AddLight(sunModel);

	auto sunController = std::make_shared<SunController>(sunModel);
	m_sceneController->AddController(sunController);

	auto torchModel = std::make_shared<LightModel>();
	torchModel->SetType(LightType::Point);
	torchModel->SetColor(Color::FromFloat(1.0f, 0.4f, 0.1f, 1.0f));
	torchModel->SetPosition({2.0f, 1.0f, 2.0f});
	torchModel->SetIntensity(2.0f);
	torchModel->SetRange(15.0f);
	m_sceneModel->AddLight(torchModel);

	auto themeController = std::make_shared<ThemeController>(themeModel);
	m_sceneController->AddController(themeController);

	auto themeView = std::make_shared<ThemeView>(themeModel, themeController);
	themeModel->RegisterObserver(themeView);
	m_sceneView->AddUiView(themeView);

	auto toastModel = std::make_shared<ToastModel>();

	m_toastController = std::make_shared<ToastController>(toastModel);
	m_sceneController->AddController(m_toastController);

	auto toastView = std::make_shared<ToastView>(toastModel, themeModel);
	toastModel->RegisterObserver(toastView);
	themeModel->RegisterObserver(toastView);
	m_sceneView->AddUiView(toastView);

	OpenglGeometryFactory geometryFactory;
	std::string vertSource = FileReader::ReadFileToString("static/shaders/phong.vert");
	std::string fragSource = FileReader::ReadFileToString("static/shaders/phong.frag");
	auto sharedShader = std::make_shared<OpenglShader>(vertSource, fragSource);

	auto planeMesh = geometryFactory.CreatePlane(100.0f, 100.0f);
	auto planeMaterial = std::make_shared<OpenglMaterial>();
	planeMaterial->SetShader(sharedShader);

	auto planeModel = std::make_shared<EntityModel>();
	planeModel->SetColor(Color::FromFloat(0.2f, 0.8f, 0.2f, 1.0f));
	planeModel->SetPosition({0.0f, -1.0f, 0.0f});
	m_sceneModel->AddEntity(planeModel);

	auto planeController = std::make_shared<EntityController>(planeModel);
	m_sceneController->AddController(planeController);

	m_sceneView->RegisterEntityVisuals(planeModel, planeMesh, planeMaterial);

	auto cubeMesh = geometryFactory.CreateCube(2.0f);
	auto cubeMaterial = std::make_shared<OpenglMaterial>();
	cubeMaterial->SetShader(sharedShader);

	auto cubeModel = std::make_shared<EntityModel>();
	cubeModel->SetColor(Color::FromFloat(0.8f, 0.2f, 0.2f, 1.0f));
	m_sceneModel->AddEntity(cubeModel);

	auto cubeController = std::make_shared<EntityController>(cubeModel);
	m_sceneController->AddController(cubeController);

	m_sceneView->RegisterEntityVisuals(cubeModel, cubeMesh, cubeMaterial);
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}