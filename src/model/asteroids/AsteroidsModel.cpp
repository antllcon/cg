#include "AsteroidsModel.h"
#include "src/system/AppConfig.h"
#include <cmath>
#include <stdexcept>

namespace
{
constexpr float FRICTION_COEFFICIENT = 0.99f;
constexpr float THRUST_POWER = 400.0f;
constexpr float ROTATION_SPEED = 4.0f;
constexpr float MAX_SPEED = 500.0f;

void AssertIsNonNegativeDeltaTime(float dt)
{
	if (dt < 0.0f)
	{
		throw std::runtime_error("Время дельты не может быть отрицательным");
	}
}

float CalculateSpeed(const Point2f& velocity)
{
	return std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

void LimitSpeed(Point2f& velocity, float maxSpeed)
{
	float currentSpeed = CalculateSpeed(velocity);

	if (currentSpeed > maxSpeed)
	{
		float ratio = maxSpeed / currentSpeed;
		velocity.x *= ratio;
		velocity.y *= ratio;
	}
}
} // namespace

AsteroidsModel::AsteroidsModel()
	: m_rotationDirection(0.0f)
{
	m_data.ship.position.x = static_cast<float>(AppConfig::WINDOW_WIDTH) / 2.0f;
	m_data.ship.position.y = static_cast<float>(AppConfig::WINDOW_HEIGHT) / 2.0f;
	m_data.ship.velocity.x = 0.0f;
	m_data.ship.velocity.y = 0.0f;
	m_data.ship.angle = -1.5707f;
	m_data.ship.isThrusting = false;
	m_data.ship.state = ShipState::Alive;

	m_data.score = 0;
	m_data.lives = 3;
	m_data.isGameOver = false;
}

void AsteroidsModel::Update(float dt)
{
	AssertIsNonNegativeDeltaTime(dt);

	if (dt == 0.0f || m_data.isGameOver)
	{
		return;
	}

	UpdateShipPhysics(dt);
	NotifyObservers();
}

void AsteroidsModel::SetThrust(bool isThrusting)
{
	if (m_data.ship.state == ShipState::Alive)
	{
		m_data.ship.isThrusting = isThrusting;
	}
}

void AsteroidsModel::SetRotation(float direction)
{
	if (m_data.ship.state == ShipState::Alive)
	{
		m_rotationDirection = direction;
	}
}

void AsteroidsModel::Shoot()
{
}

const AsteroidsData& AsteroidsModel::GetData() const
{
	return m_data;
}

AsteroidsData AsteroidsModel::GetChangedData() const
{
	return m_data;
}

void AsteroidsModel::UpdateShipPhysics(float dt)
{
	if (m_data.ship.state != ShipState::Alive)
	{
		return;
	}

	m_data.ship.angle += m_rotationDirection * ROTATION_SPEED * dt;

	if (m_data.ship.isThrusting)
	{
		m_data.ship.velocity.x += std::cos(m_data.ship.angle) * THRUST_POWER * dt;
		m_data.ship.velocity.y += std::sin(m_data.ship.angle) * THRUST_POWER * dt;
	}

	m_data.ship.velocity.x *= std::pow(FRICTION_COEFFICIENT, dt * 60.0f);
	m_data.ship.velocity.y *= std::pow(FRICTION_COEFFICIENT, dt * 60.0f);

	LimitSpeed(m_data.ship.velocity, MAX_SPEED);

	m_data.ship.position.x += m_data.ship.velocity.x * dt;
	m_data.ship.position.y += m_data.ship.velocity.y * dt;

	WrapCoordinates(m_data.ship.position);
}

void AsteroidsModel::WrapCoordinates(Point2f& position)
{
	float width = static_cast<float>(AppConfig::WINDOW_WIDTH);
	float height = static_cast<float>(AppConfig::WINDOW_HEIGHT);

	if (position.x < 0.0f)
	{
		position.x += width;
	}
	else if (position.x > width)
	{
		position.x -= width;
	}

	if (position.y < 0.0f)
	{
		position.y += height;
	}
	else if (position.y > height)
	{
		position.y -= height;
	}
}