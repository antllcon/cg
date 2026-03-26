#include "SceneView.h"
#include <stdexcept>

namespace
{
void AssertIsNotNull(const void* ptr)
{
	if (!ptr)
	{
		throw std::runtime_error("Указатель не может быть пустым в SceneView");
	}
}
} // namespace

SceneView::SceneView(std::shared_ptr<SceneModel> sceneModel)
	: m_sceneModel(std::move(sceneModel))
{
	AssertIsNotNull(m_sceneModel.get());
}

void SceneView::SetCamera(std::shared_ptr<CameraModel> camera)
{
	m_cameraModel = std::move(camera);
}

void SceneView::AddUiView(std::shared_ptr<IView> view)
{
	if (view)
	{
		m_uiViews.push_back(std::move(view));
	}
}

void SceneView::RegisterEntityVisuals(
	std::shared_ptr<EntityModel> model,
	std::shared_ptr<IMesh> mesh,
	std::shared_ptr<IMaterial> material)
{
	AssertIsNotNull(model.get());
	AssertIsNotNull(mesh.get());
	AssertIsNotNull(material.get());

	m_entityVisuals.push_back({std::move(model), std::move(mesh), std::move(material)});
}

void SceneView::Render(IRenderer& renderer) const
{
	if (m_cameraModel)
	{
		renderer.BeginFrame(m_cameraModel->GetData());
	}

	for (const auto& light : m_sceneModel->GetLights())
	{
		renderer.SubmitLight(light->GetData());
	}

	for (const auto& visual : m_entityVisuals)
	{
		const EntityData& data = visual.model->GetData();
		visual.material->SetDiffuseColor(data.color);
		renderer.SubmitMesh(visual.mesh, visual.material, data.transform);
	}

	if (m_cameraModel)
	{
		renderer.EndFrame();
	}

	renderer.BeginUI();

	for (const auto& uiView : m_uiViews)
	{
		uiView->Render(renderer);
	}

	renderer.EndUI();
}