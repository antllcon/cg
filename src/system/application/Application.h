#pragma once

#include "src/scene/SceneManager.h"
#include "src/system/renderer/IRenderer.h"
#include "src/system/window/IWindow.h"
#include <memory>

class Application final
{
public:
	Application();
	~Application() = default;

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(Application&&) = delete;

	void Run();
	void LoadScene(std::unique_ptr<SceneManager> scene);

private:
	void ProcessEvents();
	void UpdateLogic(float dt);
	void Render();

	std::unique_ptr<IWindow> m_window;
	std::unique_ptr<IRenderer> m_renderer;
	std::unique_ptr<SceneManager> m_sceneManager;
};