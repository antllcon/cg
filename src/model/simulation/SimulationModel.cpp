#include "SimulationModel.h"
#include "src/system/AppConfig.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace
{
	constexpr size_t MAX_BALLS = 50;
	constexpr float GRAVITY = 980.0f;
	constexpr float BOUNCE_DAMPING = 0.8f;
	constexpr float BALL_RADIUS = 8.0f;
	constexpr float INITIAL_SPEED = 700.0f;
	constexpr float TIME_SCALE_STEP = 0.2f;
	constexpr float MIN_TIME_SCALE = 0.2f;
	constexpr float MAX_TIME_SCALE = 3.0f;

	void AssertIsPositive(float value)
	{
		if (value < 0.0f)
		{
			throw std::runtime_error("Время дельты должно быть строго положительным");
		}
	}

	float CalculateDistance(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;

		return std::sqrt(dx * dx + dy * dy);
	}

	sf::Vector2f NormalizeVector(const sf::Vector2f& v)
	{
		float length = std::sqrt(v.x * v.x + v.y * v.y);

		if (length == 0.0f)
		{
			return {0.0f, 0.0f};
		}

		return {v.x / length, v.y / length};
	}

	sf::Vector2f RotateVector(const sf::Vector2f& v, float angle)
	{
		float c = std::cos(angle);
		float s = std::sin(angle);

		return {v.x * c - v.y * s, v.x * s + v.y * c};
	}

	float DotProduct(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	void ResolveBallToBallCollision(BallData& b1, BallData& b2)
	{
		float dist = CalculateDistance(b1.position, b2.position);
		float minDist = b1.radius + b2.radius;

		if (dist < minDist && dist > 0.0f)
		{
			sf::Vector2f normal = {(b2.position.x - b1.position.x) / dist, (b2.position.y - b1.position.y) / dist};
			float overlap = minDist - dist;

			b1.position.x -= normal.x * overlap * 0.5f;
			b1.position.y -= normal.y * overlap * 0.5f;
			b2.position.x += normal.x * overlap * 0.5f;
			b2.position.y += normal.y * overlap * 0.5f;

			sf::Vector2f relativeVelocity = {b2.velocity.x - b1.velocity.x, b2.velocity.y - b1.velocity.y};
			float velocityAlongNormal = DotProduct(relativeVelocity, normal);

			if (velocityAlongNormal < 0.0f)
			{
				float impulse = -(1.0f + BOUNCE_DAMPING) * velocityAlongNormal / 2.0f;

				b1.velocity.x -= impulse * normal.x;
				b1.velocity.y -= impulse * normal.y;
				b2.velocity.x += impulse * normal.x;
				b2.velocity.y += impulse * normal.y;
			}
		}
	}

	void ResolveBallToObstacleCollision(BallData& ball, const RectObstacleData& obstacle)
	{
		sf::Vector2f localCircleCenter = {ball.position.x - obstacle.center.x, ball.position.y - obstacle.center.y};
		localCircleCenter = RotateVector(localCircleCenter, -obstacle.rotationAngle);

		float halfW = obstacle.size.x / 2.0f;
		float halfH = obstacle.size.y / 2.0f;

		sf::Vector2f closestPoint = {
			std::clamp(localCircleCenter.x, -halfW, halfW),
			std::clamp(localCircleCenter.y, -halfH, halfH)
		};

		sf::Vector2f localDelta = {localCircleCenter.x - closestPoint.x, localCircleCenter.y - closestPoint.y};
		float distSq = localDelta.x * localDelta.x + localDelta.y * localDelta.y;

		if (distSq > 0.0f && distSq <= ball.radius * ball.radius)
		{
			float dist = std::sqrt(distSq);
			sf::Vector2f localNormal = {localDelta.x / dist, localDelta.y / dist};

			sf::Vector2f worldNormal = RotateVector(localNormal, obstacle.rotationAngle);

			float overlap = ball.radius - dist;
			ball.position.x += worldNormal.x * overlap;
			ball.position.y += worldNormal.y * overlap;

			float velocityAlongNormal = DotProduct(ball.velocity, worldNormal);

			if (velocityAlongNormal < 0.0f)
			{
				float impulse = -(1.0f + BOUNCE_DAMPING) * velocityAlongNormal;
				ball.velocity.x += impulse * worldNormal.x;
				ball.velocity.y += impulse * worldNormal.y;
			}
		}
	}

	void ResolveBallToWallsCollision(BallData& ball, float width)
	{
		if (ball.position.x - ball.radius < 0.0f)
		{
			ball.position.x = ball.radius;
			ball.velocity.x = -ball.velocity.x * BOUNCE_DAMPING;
			ball.velocity.y *= BOUNCE_DAMPING;
		}
		else if (ball.position.x + ball.radius > width)
		{
			ball.position.x = width - ball.radius;
			ball.velocity.x = -ball.velocity.x * BOUNCE_DAMPING;
			ball.velocity.y *= BOUNCE_DAMPING;
		}

		if (ball.position.y - ball.radius < 0.0f)
		{
			ball.position.y = ball.radius;
			ball.velocity.y = -ball.velocity.y * BOUNCE_DAMPING;
			ball.velocity.x *= BOUNCE_DAMPING;
		}
	}

	std::vector<RectObstacleData> CreateInitialObstacles()
	{
		std::vector<RectObstacleData> obstacles;

		obstacles.push_back({{200.0f, 150.0f}, {80.0f, 80.0f}, 0.785f});
		obstacles.push_back({{100.0f, 250.0f}, {100.0f, 20.0f}, 0.5f});
		obstacles.push_back({{300.0f, 250.0f}, {100.0f, 20.0f}, -0.5f});
		obstacles.push_back({{80.0f, 500.0f}, {120.0f, 20.0f}, 0.8f});
		obstacles.push_back({{320.0f, 500.0f}, {120.0f, 20.0f}, -0.8f});
		obstacles.push_back({{200.0f, 600.0f}, {150.0f, 20.0f}, 0.0f});
		obstacles.push_back({{100.0f, 700.0f}, {80.0f, 20.0f}, -0.4f});
		obstacles.push_back({{300.0f, 700.0f}, {80.0f, 20.0f}, 0.4f});
		obstacles.push_back({{200.0f, 780.0f}, {60.0f, 60.0f}, 0.785f});
		obstacles.push_back({{50.0f, 350.0f}, {100.0f, 20.0f}, 1.57f});
		obstacles.push_back({{350.0f, 350.0f}, {100.0f, 20.0f}, 1.57f});

		return obstacles;
	}
}

SimulationModel::SimulationModel()
	: m_timeScale(1.0f)
{
	m_data.cannon.position = {static_cast<float>(AppConfig::WINDOW_WIDTH) / 2.0f, static_cast<float>(AppConfig::WINDOW_HEIGHT) / 2.0f};
	m_data.cannon.targetDirection = {0.0f, -1.0f};
	m_data.obstacles = CreateInitialObstacles();
}

void SimulationModel::Update(float dt)
{
	AssertIsPositive(dt);

	if (dt == 0.0f)
	{
		return;
	}

	float scaledDt = dt * m_timeScale;

	ApplyGravity(scaledDt);
	MoveBalls(scaledDt);
	ResolveCollisions();
	RemoveFallenBalls();

	NotifyObservers();
}

void SimulationModel::SetTarget(float x, float y)
{
	sf::Vector2f direction = {x - m_data.cannon.position.x, y - m_data.cannon.position.y};
	m_data.cannon.targetDirection = NormalizeVector(direction);

	NotifyObservers();
}

void SimulationModel::Shoot()
{
	if (m_data.balls.size() >= MAX_BALLS)
	{
		return;
	}

	BallData newBall;
	newBall.position = m_data.cannon.position;
	newBall.velocity = {m_data.cannon.targetDirection.x * INITIAL_SPEED, m_data.cannon.targetDirection.y * INITIAL_SPEED};
	newBall.radius = BALL_RADIUS;

	m_data.balls.push_back(newBall);

	NotifyObservers();
}

void SimulationModel::IncreaseTimeScale()
{
	if (m_timeScale + TIME_SCALE_STEP <= MAX_TIME_SCALE)
	{
		m_timeScale += TIME_SCALE_STEP;
	}
}

void SimulationModel::DecreaseTimeScale()
{
	if (m_timeScale - TIME_SCALE_STEP >= MIN_TIME_SCALE)
	{
		m_timeScale -= TIME_SCALE_STEP;
	}
}

const SimulationData& SimulationModel::GetData() const
{
	return m_data;
}

SimulationData SimulationModel::GetChangedData() const
{
	return m_data;
}

void SimulationModel::ApplyGravity(float dt)
{
	for (auto& ball : m_data.balls)
	{
		ball.velocity.y += GRAVITY * dt;
	}
}

void SimulationModel::MoveBalls(float dt)
{
	for (auto& ball : m_data.balls)
	{
		ball.position.x += ball.velocity.x * dt;
		ball.position.y += ball.velocity.y * dt;
	}
}

void SimulationModel::ResolveCollisions()
{
	for (size_t i = 0; i < m_data.balls.size(); ++i)
	{
		ResolveBallToWallsCollision(m_data.balls[i], static_cast<float>(AppConfig::WINDOW_WIDTH));

		for (const auto& obstacle : m_data.obstacles)
		{
			ResolveBallToObstacleCollision(m_data.balls[i], obstacle);
		}

		for (size_t j = i + 1; j < m_data.balls.size(); ++j)
		{
			ResolveBallToBallCollision(m_data.balls[i], m_data.balls[j]);
		}
	}
}

void SimulationModel::RemoveFallenBalls()
{
	auto it = m_data.balls.begin();

	while (it != m_data.balls.end())
	{
		if (it->position.y - it->radius > static_cast<float>(AppConfig::WINDOW_HEIGHT))
		{
			it = m_data.balls.erase(it);
		}
		else
		{
			++it;
		}
	}
}