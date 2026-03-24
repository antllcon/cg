#include "CubeView.h"
#include "src/controller/cube/CubeController.h"

CubeView::CubeView(
	std::shared_ptr<CubeModel> model,
	std::shared_ptr<CubeController> controller,
	std::shared_ptr<IMesh> mesh,
	std::shared_ptr<IMaterial> material)
	: m_controller(std::move(controller))
	, m_mesh(std::move(mesh))
	, m_material(std::move(material))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}
}

void CubeView::HandleEvent(const Event& event)
{
	m_controller->HandleEvent(event);
}

void CubeView::Render(IRenderer& renderer) const
{
	renderer.SubmitMesh(m_mesh, m_material, m_data.transform);
}

void CubeView::Update(const CubeData& data, IObservable<CubeData>*)
{
	m_data = data;
	m_material->SetDiffuseColor(m_data.color);
}