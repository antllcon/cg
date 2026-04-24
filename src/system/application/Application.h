#pragma once
#include "src/core/interfaces/IAudioManager.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/core/interfaces/IWindow.h"
#include "src/scene/Scene.h"
#include <memory>

class Application final : public std::enable_shared_from_this<Application>
{
public:
	Application(
		std::unique_ptr<IWindow> window,
		std::unique_ptr<IRenderer> renderer,
		std::unique_ptr<IAudioManager> audioManager);
	~Application() = default;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	void Init();
	void Run();

private:
	void ProcessEvents();
	void UpdateLogic(float dt);
	void Render();
	void LoadScene(std::unique_ptr<Scene> scene);

	std::unique_ptr<IWindow> m_window;
	std::unique_ptr<IRenderer> m_renderer;
	std::unique_ptr<IAudioManager> m_audioManager;
	std::unique_ptr<Scene> m_scene;
};
