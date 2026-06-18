#include "View.h"
#include "src/system/renderer/IRenderer.h"

View::View(std::shared_ptr<Model> model)
	: m_model(model)
{
}

void View::HandleEvent(const Event&)
{
}

void View::Render(IRenderer& renderer) const
{
	renderer.DrawProcedural(m_model->GetState());
}

void View::Update(const ModelData&, IObservable<ModelData>*)
{
}