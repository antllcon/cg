#include "EntityView.h"
#include "src/controller/entity/EntityController.h"

EntityView::EntityView(
	std::shared_ptr<EntityModel> model,
	std::shared_ptr<EntityController> controller,
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

void EntityView::HandleEvent(const Event& event)
{
	m_controller->HandleEvent(event);
}

void EntityView::Render(IRenderer& renderer) const
{
	renderer.SubmitMesh(m_mesh, m_material, m_data.transform);
}

void EntityView::Update(const EntityData& data, IObservable<EntityData>*)
{
	m_data = data;
	m_material->SetDiffuseColor(m_data.color);
}