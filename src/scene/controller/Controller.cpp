#include "Controller.h"
#include "src/utils/dialog/SystemDialogs.h"

Controller::Controller(std::shared_ptr<Model> model)
	: m_model(model)
{
}

void Controller::Update(float)
{
}

void Controller::HandleEvent(const Event&)
{
}

void Controller::OnOpenImageRequested()
{
	auto pathOpt = SystemDialogs::OpenFile("Выберите изображение", Image::DIALOG_FILTER);

	if (pathOpt.has_value())
	{
		m_model->LoadImage(pathOpt.value());
	}
}

void Controller::OnCloseImageRequested()
{
	m_model->CloseImage();
}

void Controller::OnSaveImageRequested()
{
	auto pathOpt = SystemDialogs::SaveFile("Сохранить изображение как", Image::DIALOG_FILTER);

	if (pathOpt.has_value())
	{
		m_model->SaveImage(pathOpt.value());
	}
}

void Controller::ToggleFilter()
{
	m_model->ToggleFilter();
}

void Controller::IncreaseFilterRadius()
{
	m_model->IncreaseFilterRadius();
}

void Controller::DecreaseFilterRadius()
{
	m_model->DecreaseFilterRadius();
}