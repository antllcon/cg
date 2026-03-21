#include "AsteroidsController.h"
#include "src/core/types/event/EventHandling.h"

namespace
{
constexpr auto SOUND_SHOOT = "static/shoot.mp3";
constexpr auto SOUND_BANG = "static/punch.mp3";
constexpr auto SOUND_EXPLOSION = "static/faaah.mp3";
constexpr auto SOUND_THRUST = "static/fire-burning.mp3";
} // namespace

AsteroidsController::AsteroidsController(std::shared_ptr<AsteroidsModel> model, IAudioManager& audioManager)
	: m_model(std::move(model))
	, m_audioManager(audioManager)
{
	m_audioManager.PreloadSound(SOUND_SHOOT);
	m_audioManager.PreloadSound(SOUND_BANG);
	m_audioManager.PreloadSound(SOUND_EXPLOSION);
	m_audioManager.PreloadSound(SOUND_THRUST);
}

void AsteroidsController::Update(float dt)
{
	int previousScore = m_model->GetData().state.score;
	int previousLives = m_model->GetData().state.lives;

	m_model->UpdatePhysics(dt);

	int currentScore = m_model->GetData().state.score;
	int currentLives = m_model->GetData().state.lives;

	if (currentScore > previousScore)
	{
		m_audioManager.PlaySoundFile(SOUND_BANG);
	}

	if (currentLives < previousLives)
	{
		m_audioManager.PlaySoundFile(SOUND_EXPLOSION);
	}
}

void AsteroidsController::HandleEvent(const Event& event)
{
	std::visit(Overload{
				[this](const KeyPressedEvent& e) {
				   if (e.code == KeyCode::W || e.code == KeyCode::Up)
				   {
					  if (!m_model->GetData().ship.isAccelerating)
					  {
						  m_audioManager.PlaySoundFile(SOUND_THRUST);
					  }
					  m_model->SetShipAcceleration(true);
				   }
				   else if (e.code == KeyCode::A || e.code == KeyCode::Left)
				   {
					  m_model->SetShipRotationState(RotationDirection::Left);
				   }
				   else if (e.code == KeyCode::D || e.code == KeyCode::Right)
				   {
					  m_model->SetShipRotationState(RotationDirection::Right);
				   }
				   else if (e.code == KeyCode::Space)
				   {
					  m_model->Shoot();
					  m_audioManager.PlaySoundFile(SOUND_SHOOT);
				   }
				   else if (e.code == KeyCode::Enter)
				   {
					  if (m_model->GetData().state.isGameOver)
					  {
						 m_model->RestartGame();
					  }
				   }
				},
				[this](const KeyReleasedEvent& e) {
				   if (e.code == KeyCode::W || e.code == KeyCode::Up)
				   {
					  m_model->SetShipAcceleration(false);
				   }
				   else if (e.code == KeyCode::A || e.code == KeyCode::Left ||
							e.code == KeyCode::D || e.code == KeyCode::Right)
				   {
					  m_model->SetShipRotationState(RotationDirection::None);
				   }
				},
				[](const auto&) {
				}},
	   event);
}