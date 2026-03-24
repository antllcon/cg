#pragma once
#include "src/core/interfaces/IMaterial.h"
#include "src/core/interfaces/IMesh.h"
#include "src/core/interfaces/IRenderer.h"
#include "src/model/cube/CubeModel.h"
#include "src/system/Observer.h"
#include "src/view/IView.h"
#include <memory>

class CubeController;

class CubeView final
	: public IView
	, public IObserver<CubeData>
{
public:
	CubeView(
		std::shared_ptr<CubeModel> model,
		std::shared_ptr<CubeController> controller,
		std::shared_ptr<IMesh> mesh,
		std::shared_ptr<IMaterial> material);

	void HandleEvent(const Event& event) override;
	void Render(IRenderer& renderer) const override;
	void Update(const CubeData& data, IObservable<CubeData>* subject) override;

private:
	std::shared_ptr<CubeController> m_controller;
	std::shared_ptr<IMesh> m_mesh;
	std::shared_ptr<IMaterial> m_material;
	CubeData m_data;
};