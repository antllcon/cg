#include "Controller.h"
#include "src/utils/dialog/SystemDialogs.h"

Controller::Controller(std::shared_ptr<Model> model)
	: m_model(model)
{
}

void Controller::Update(float)
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

void Controller::IncreaseFilterRadius()
{
	uint8_t currentRadius = m_model->GetMedianRadius();
	if (currentRadius < FilterConfig::MAX_RADIUS)
	{
		m_model->SetMedianRadius(currentRadius + 1);
	}
}

void Controller::DecreaseFilterRadius()
{
	uint8_t currentRadius = m_model->GetMedianRadius();
	if (currentRadius > FilterConfig::MIN_RADIUS)
	{
		m_model->SetMedianRadius(currentRadius - 1);
	}
}