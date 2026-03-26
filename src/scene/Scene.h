#pragma once
#include "src/controller/scene/SceneController.h"
#include "src/core/interfaces/IAudioManager.h"
#include "src/core/types/event/Event.h"
#include "src/model/scene/SceneModel.h"
#include "src/model/theme/ThemeModel.h"
#include "src/view/scene/SceneView.h"
#include <memory>

class IRenderer;
class IWindow;

class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

	virtual void Init(std::shared_ptr<ThemeModel> themeModel, IAudioManager& audioManager, IWindow& window);

	virtual void ProcessEvents(const Event& event);
	virtual void Update(float dt);
	virtual void Render(IRenderer& renderer) const;
	virtual void OnException(const std::exception& e);

protected:
	std::shared_ptr<SceneModel> m_sceneModel;
	std::shared_ptr<SceneController> m_sceneController;
	std::shared_ptr<SceneView> m_sceneView;
};