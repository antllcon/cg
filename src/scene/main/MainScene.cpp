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
#include "src/view/fps/FpsView.h"
#include "src/view/theme/ThemeView.h"
#include "src/view/toast/ToastView.h"

void MainScene::Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager&, IWindow& window)
{
	auto windowController = std::make_shared<WindowController>(window);
	m_sceneController->AddController(windowController);

	m_cameraModel = std::make_shared<CameraModel>();
	m_sceneView->SetCamera(m_cameraModel);

	auto cameraController = std::make_shared<CameraController>(m_cameraModel, window);
	m_sceneController->AddController(cameraController);

	auto cameraView = std::make_shared<CameraView>(m_cameraModel, cameraController);
	m_cameraModel->RegisterObserver(cameraView);
	m_sceneView->AddUiView(cameraView);

	auto sunModel = std::make_shared<SunModel>();
	m_sceneModel->AddLight(sunModel);

	auto sunController = std::make_shared<SunController>(sunModel);
	sunController->SetTimeScale(0.0f);
	sunController->SetTimeOfDay(12.0f);
	m_sceneController->AddController(sunController);

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

	m_fpsModel = std::make_shared<FpsModel>();
	auto fpsView = std::make_shared<FpsView>(m_fpsModel);
	m_fpsModel->RegisterObserver(fpsView);
	m_sceneView->AddUiView(fpsView);

	OpenglGeometryFactory geometryFactory;

	std::string vertSource = FileReader::ReadFileToString("static/shaders/phong.vert");
	std::string fragSource = FileReader::ReadFileToString("static/shaders/phong.frag");
	auto sharedShader = std::make_shared<OpenglShader>(vertSource, fragSource);

	auto planeMesh = geometryFactory.CreatePlane(100.0f, 100.0f);
	auto cubeMesh = geometryFactory.CreateCube(2.0f);
	auto barrelMesh = geometryFactory.CreateFromObj("static/models/barrel.obj");

	auto planeMaterial = std::make_shared<OpenglMaterial>();
	planeMaterial->SetShader(sharedShader);

	auto planeModel = std::make_shared<EntityModel>();
	planeModel->SetPosition({0.0f, -1.0f, 0.0f});
	planeModel->SetColor(Color::FromRGBA(166, 181, 128));
	m_sceneModel->AddEntity(planeModel);
	m_sceneView->RegisterEntityVisuals(planeModel, planeMesh, planeMaterial);

	auto cubeMaterial = std::make_shared<OpenglMaterial>();
	cubeMaterial->SetShader(sharedShader);

	auto cubeModel = std::make_shared<EntityModel>();
	cubeModel->SetPosition({0.0f, 0.0f, 0.0f});
	cubeModel->SetColor(Color::FromFloat(0.4f, 0.2f, 0.2f, 1.0f));
	m_sceneModel->AddEntity(cubeModel);
	m_sceneView->RegisterEntityVisuals(cubeModel, cubeMesh, cubeMaterial);

	auto barrel1Material = std::make_shared<OpenglMaterial>();
	barrel1Material->SetShader(sharedShader);

	auto barrel1 = std::make_shared<EntityModel>();
	barrel1->SetPosition({2.0f, 0.0f, 0.0f});
	barrel1->SetScale({0.05f, 0.05f, 0.05f});
	barrel1->SetColor(Color::FromFloat(0.5f, 0.3f, 0.1f, 1.0f));
	m_sceneModel->AddEntity(barrel1);
	m_sceneView->RegisterEntityVisuals(barrel1, barrelMesh, barrel1Material);

	auto barrel2Material = std::make_shared<OpenglMaterial>();
	barrel2Material->SetShader(sharedShader);

	auto barrel2 = std::make_shared<EntityModel>();
	barrel2->SetPosition({-2.0f, 0.0f, 0.0f});
	barrel2->SetScale({0.05f, 0.05f, 0.05f});
	barrel2->SetColor(Color::FromFloat(0.6f, 0.4f, 0.2f, 1.0f));
	m_sceneModel->AddEntity(barrel2);
	m_sceneView->RegisterEntityVisuals(barrel2, barrelMesh, barrel2Material);
}

void MainScene::Update(float dt)
{
	Scene::Update(dt);

	if (m_fpsModel)
	{
		m_fpsModel->Update(dt);
	}
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}