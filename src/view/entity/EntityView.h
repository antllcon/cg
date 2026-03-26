#pragma once
#include "src/core/interfaces/IMaterial.h"
#include "src/core/interfaces/IMesh.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/model/entity/EntityModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>

class EntityController;

class EntityView final
	: public IView
	, public IObserver<EntityData>
{
public:
	EntityView(
		std::shared_ptr<EntityModel> model,
		std::shared_ptr<EntityController> controller,
		std::shared_ptr<IMesh> mesh,
		std::shared_ptr<IMaterial> material);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const EntityData& data, IObservable<EntityData>* subject) override;

private:
	std::shared_ptr<EntityController> m_controller;
	std::shared_ptr<IMesh> m_mesh;
	std::shared_ptr<IMaterial> m_material;
	EntityData m_data;
};