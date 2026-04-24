#include "MainScene.h"
#include "src/controller/camera/CameraController.h"
#include "src/controller/entity/EntityController.h"
#include "src/controller/sun/SunController.h"
#include "src/controller/window/WindowController.h"
#include "src/core/utils/FileReader.h"
#include "src/system/AppConfig.h"
#include "src/system/render/opengl/OpenglGeometryFactory.h"
#include "src/system/render/opengl/OpenglMaterial.h"
#include "src/system/render/opengl/OpenglShader.h"
#include "src/view/camera/CameraView.h"
#include "src/view/fps/FpsView.h"
#include "src/view/toast/ToastView.h"

#include <numbers>

namespace
{
constexpr float TORCH_INTENSITY = 1.5f;
constexpr float TORCH_RANGE = 15.0f;
constexpr float SUN_DEFAULT_TIME = 12.0f;

constexpr auto SHADER_VERT_PATH = "static/shaders/phong.vert";
constexpr auto SHADER_FRAG_PATH = "static/shaders/phong.frag";
constexpr auto BACKGROUND_MUSIC_PATH = "static/audio/my-mommy.mp3";
} // namespace

void MainScene::Init(IAudioManager& audioManager, IWindow& window)
{
	auto windowController = std::make_shared<WindowController>(window);
	m_sceneController->AddController(windowController);

	m_cameraModel = std::make_shared<CameraModel>();
	m_sceneView->SetCamera(m_cameraModel);

	m_mazeModel = std::make_shared<MazeModel>();

	auto cameraController = std::make_shared<CameraController>(m_cameraModel, m_mazeModel, window, audioManager);
	m_sceneController->AddController(cameraController);

	auto cameraView = std::make_shared<CameraView>(m_cameraModel, cameraController);
	m_cameraModel->RegisterObserver(cameraView);
	m_sceneView->AddUiView(cameraView);

	auto sunModel = std::make_shared<SunModel>();
	m_sceneModel->AddLight(sunModel);

	m_torchModel = std::make_shared<LightModel>();
	m_torchModel->SetType(LightType::Point);
	m_torchModel->SetColor(Color::FromFloat(1.0f, 0.8f, 0.5f, 1.0f));
	m_torchModel->SetIntensity(TORCH_INTENSITY);
	m_torchModel->SetRange(TORCH_RANGE);
	m_sceneModel->AddLight(m_torchModel);

	auto sunController = std::make_shared<SunController>(sunModel);
	sunController->SetTimeScale(0.0f);
	sunController->SetTimeOfDay(SUN_DEFAULT_TIME);
	m_sceneController->AddController(sunController);

	auto toastModel = std::make_shared<ToastModel>();
	m_toastController = std::make_shared<ToastController>(toastModel);
	m_sceneController->AddController(m_toastController);

	auto toastView = std::make_shared<ToastView>(toastModel);
	toastModel->RegisterObserver(toastView);
	m_sceneView->AddUiView(toastView);

	m_fpsModel = std::make_shared<FpsModel>();
	auto fpsView = std::make_shared<FpsView>(m_fpsModel);
	m_fpsModel->RegisterObserver(fpsView);
	m_sceneView->AddUiView(fpsView);

	GenerateLevel();

	audioManager.PlayMusic(BACKGROUND_MUSIC_PATH);
}

void MainScene::Update(float dt)
{
	Scene::Update(dt);

	if (m_fpsModel)
	{
		m_fpsModel->Update(dt);
	}

	if (m_torchModel && m_cameraModel)
	{
		m_torchModel->SetPosition(m_cameraModel->GetData().position);
	}
}

void MainScene::OnException(const std::exception& e)
{
	if (m_toastController)
	{
		m_toastController->ShowError(e.what());
	}
}

void MainScene::GenerateLevel()
{
	OpenglGeometryFactory geometryFactory;

	std::string vertSource = FileReader::ReadFileToString(SHADER_VERT_PATH);
	std::string fragSource = FileReader::ReadFileToString(SHADER_FRAG_PATH);
	auto sharedShader = std::make_shared<OpenglShader>(vertSource, fragSource);

	const auto& layout = m_mazeModel->GetLayout();
	float blockSize = m_mazeModel->GetBlockSize();
	float mazeWidth = static_cast<float>(layout[0].size()) * blockSize;
	float mazeDepth = static_cast<float>(layout.size()) * blockSize;
	float centerX = mazeWidth / 2.0f - blockSize / 2.0f;
	float centerZ = mazeDepth / 2.0f - blockSize / 2.0f;

	auto planeMesh = geometryFactory.CreatePlane(mazeWidth, mazeDepth);
	auto cubeMesh = geometryFactory.CreateCube(blockSize);

	auto floorMaterial = std::make_shared<OpenglMaterial>();
	floorMaterial->SetShader(sharedShader);
	floorMaterial->SetSpecularColor(Color::FromFloat(0.0f, 0.0f, 0.0f, 0.0f));

	auto floorModel = std::make_shared<EntityModel>();
	floorModel->SetPosition({centerX, -blockSize / 2.0f, centerZ});
	floorModel->SetColor(Color::FromRGBA(127, 0, 255));
	m_sceneModel->AddEntity(floorModel);
	m_sceneView->RegisterEntityVisuals(floorModel, planeMesh, floorMaterial);

	auto ceilingMaterial = std::make_shared<OpenglMaterial>();
	ceilingMaterial->SetShader(sharedShader);
	ceilingMaterial->SetSpecularColor(Color::FromFloat(0.0f, 0.0f, 0.0f, 0.0f));

	auto ceilingModel = std::make_shared<EntityModel>();
	ceilingModel->SetPosition({centerX, blockSize / 2.0f, centerZ});
	ceilingModel->Rotate(std::numbers::pi_v<float>, {1.0f, 0.0f, 0.0f});
	ceilingModel->SetColor(Color::FromRGBA(160, 120, 120));
	m_sceneModel->AddEntity(ceilingModel);
	m_sceneView->RegisterEntityVisuals(ceilingModel, planeMesh, ceilingMaterial);

	auto wallMaterial = std::make_shared<OpenglMaterial>();
	wallMaterial->SetShader(sharedShader);

	for (size_t row = 0; row < layout.size(); ++row)
	{
		for (size_t col = 0; col < layout[row].size(); ++col)
		{
			if (layout[row][col] == '#')
			{
				auto wallModel = std::make_shared<EntityModel>();
				wallModel->SetPosition({static_cast<float>(col) * blockSize,
					0.0f,
					static_cast<float>(row) * blockSize});
				wallModel->SetColor(Color::FromRGBA(166, 181, 128));
				m_sceneModel->AddEntity(wallModel);
				m_sceneView->RegisterEntityVisuals(wallModel, cubeMesh, wallMaterial);
			}
		}
	}
}
