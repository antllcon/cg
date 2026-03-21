#include "AsteroidsModel.h"
#include "src/system/AppConfig.h"
#include <numbers>
#include <random>

namespace
{
constexpr float SHIP_ACCELERATION = 400.0f;
constexpr float SHIP_MAX_SPEED = 500.0f;
constexpr float SHIP_FRICTION = 0.98f;
constexpr float SHIP_ROTATION_SPEED = 4.0f;

constexpr float BULLET_SPEED = 700.0f;
constexpr float BULLET_LIFETIME = 1.5f;
constexpr float SHOOT_COOLDOWN = 0.25f;

constexpr float RESPAWN_DELAY = 2.0f;
constexpr int INITIAL_ASTEROIDS = 6;

constexpr int POINTS_PER_LARGE = 20;
constexpr int POINTS_PER_MEDIUM = 50;
constexpr int POINTS_PER_SMALL = 100;

void AssertIsPositive(float value)
{
	if (value <= 0.0f)
	{
		throw std::runtime_error("Значение должно быть строго больше нуля");
	}
}

float GetRandomFloat(float min, float max)
{
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(rng);
}

Point2f RotateVector(const Point2f& vec, float angle)
{
	float cosA = std::cos(angle);
	float sinA = std::sin(angle);
	return Point2f{
		vec.x * cosA - vec.y * sinA,
		vec.x * sinA + vec.y * cosA};
}

float CalculateTriangleSign(const Point2f& p1, const Point2f& p2, const Point2f& p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool IsPointInTriangle(const Point2f& pt, const Point2f& v1, const Point2f& v2, const Point2f& v3)
{
	float d1 = CalculateTriangleSign(pt, v1, v2);
	float d2 = CalculateTriangleSign(pt, v2, v3);
	float d3 = CalculateTriangleSign(pt, v3, v1);

	bool hasNeg = d1 < 0.0f || d2 < 0.0f || d3 < 0.0f;
	bool hasPos = d1 > 0.0f || d2 > 0.0f || d3 > 0.0f;

	return !(hasNeg && hasPos);
}

bool IsPointInPolygon(const Point2f& pt, const std::vector<Point2f>& localVertices, const Point2f& position, float angle)
{
	if (localVertices.size() < 3)
	{
		return false;
	}

	Point2f center = position;
	std::vector<Point2f> worldVertices;
	worldVertices.reserve(localVertices.size());

	for (const auto& local : localVertices)
	{
		Point2f rotated = RotateVector(local, angle);
		worldVertices.push_back({position.x + rotated.x, position.y + rotated.y});
	}

	for (size_t i = 0; i < worldVertices.size(); ++i)
	{
		Point2f v1 = worldVertices[i];
		Point2f v2 = worldVertices[(i + 1) % worldVertices.size()];

		if (IsPointInTriangle(pt, center, v1, v2))
		{
			return true;
		}
	}

	return false;
}

std::vector<Point2f> GenerateShipVertices()
{
	return {
		{15.0f, 0.0f},
		{-10.0f, -10.0f},
		{-5.0f, 0.0f},
		{-10.0f, 10.0f}};
}

std::vector<Point2f> GenerateAsteroidVertices(AsteroidSize size)
{
	float radius = 0.0f;
	int segments = 0;

	switch (size)
	{
	case AsteroidSize::Large:
		radius = 40.0f;
		segments = 12;
		break;
	case AsteroidSize::Medium:
		radius = 20.0f;
		segments = 10;
		break;
	case AsteroidSize::Small:
		radius = 10.0f;
		segments = 8;
		break;
	}

	std::vector<Point2f> vertices;
	vertices.reserve(segments);

	float angleStep = 2.0f * std::numbers::pi_v<float> / segments;

	for (int i = 0; i < segments; ++i)
	{
		float angle = i * angleStep;
		float currentRadius = radius * GetRandomFloat(0.8f, 1.2f);
		vertices.push_back({currentRadius * std::cos(angle), currentRadius * std::sin(angle)});
	}

	return vertices;
}

AsteroidData CreateAsteroid(const Point2f& position, AsteroidSize size)
{
	AsteroidData asteroid;
	asteroid.position = position;
	asteroid.size = size;
	asteroid.localVertices = GenerateAsteroidVertices(size);
	asteroid.angle = GetRandomFloat(0.0f, 2.0f * std::numbers::pi_v<float>);
	asteroid.rotationSpeed = GetRandomFloat(-2.0f, 2.0f);

	float speed = 0.0f;
	switch (size)
	{
	case AsteroidSize::Large:
		speed = GetRandomFloat(30.0f, 60.0f);
		break;
	case AsteroidSize::Medium:
		speed = GetRandomFloat(70.0f, 110.0f);
		break;
	case AsteroidSize::Small:
		speed = GetRandomFloat(120.0f, 160.0f);
		break;
	}

	float moveAngle = GetRandomFloat(0.0f, 2.0f * std::numbers::pi_v<float>);
	asteroid.velocity = {speed * std::cos(moveAngle), speed * std::sin(moveAngle)};

	return asteroid;
}

void SpawnInitialAsteroids(std::vector<AsteroidData>& asteroids)
{
	asteroids.clear();
	for (int i = 0; i < INITIAL_ASTEROIDS; ++i)
	{
		Point2f pos{
			GetRandomFloat(0.0f, AppConfig::WINDOW_WIDTH),
			GetRandomFloat(0.0f, AppConfig::WINDOW_HEIGHT)};
		asteroids.push_back(CreateAsteroid(pos, AsteroidSize::Large));
	}
}

void WrapCoordinates(Point2f& position)
{
	if (position.x < 0.0f)
	{
		position.x += AppConfig::WINDOW_WIDTH;
	}
	else if (position.x > AppConfig::WINDOW_WIDTH)
	{
		position.x -= AppConfig::WINDOW_WIDTH;
	}

	if (position.y < 0.0f)
	{
		position.y += AppConfig::WINDOW_HEIGHT;
	}
	else if (position.y > AppConfig::WINDOW_HEIGHT)
	{
		position.y -= AppConfig::WINDOW_HEIGHT;
	}
}

void UpdateShipPhysics(ShipData& ship, RotationDirection rotDir, float dt)
{
	if (!ship.isAlive)
	{
		return;
	}

	if (rotDir == RotationDirection::Left)
	{
		ship.angle -= SHIP_ROTATION_SPEED * dt;
	}
	else if (rotDir == RotationDirection::Right)
	{
		ship.angle += SHIP_ROTATION_SPEED * dt;
	}

	if (ship.isAccelerating)
	{
		Point2f thrust = RotateVector({SHIP_ACCELERATION * dt, 0.0f}, ship.angle);
		ship.velocity.x += thrust.x;
		ship.velocity.y += thrust.y;
	}

	ship.velocity.x *= std::pow(SHIP_FRICTION, dt * 60.0f);
	ship.velocity.y *= std::pow(SHIP_FRICTION, dt * 60.0f);

	float currentSpeed = std::sqrt(ship.velocity.x * ship.velocity.x + ship.velocity.y * ship.velocity.y);
	if (currentSpeed > SHIP_MAX_SPEED)
	{
		float ratio = SHIP_MAX_SPEED / currentSpeed;
		ship.velocity.x *= ratio;
		ship.velocity.y *= ratio;
	}

	ship.position.x += ship.velocity.x * dt;
	ship.position.y += ship.velocity.y * dt;

	WrapCoordinates(ship.position);
}

void UpdateAsteroidsPhysics(std::vector<AsteroidData>& asteroids, float dt)
{
	for (auto& asteroid : asteroids)
	{
		asteroid.position.x += asteroid.velocity.x * dt;
		asteroid.position.y += asteroid.velocity.y * dt;
		asteroid.angle += asteroid.rotationSpeed * dt;
		WrapCoordinates(asteroid.position);
	}
}

void UpdateBulletsPhysics(std::vector<BulletData>& bullets, float dt)
{
	for (auto& [position, velocity, timeToLive] : bullets)
	{
		position.x += velocity.x * dt;
		position.y += velocity.y * dt;
		timeToLive -= dt;

		if (position.x < 0.0f || position.x > AppConfig::WINDOW_WIDTH || position.y < 0.0f || position.y > AppConfig::WINDOW_HEIGHT)
		{
			timeToLive = -1.0f;
		}
	}

	for (size_t i = 0; i < bullets.size();)
	{
		if (bullets[i].timeToLive <= 0.0f)
		{
			bullets[i] = bullets.back();
			bullets.pop_back();
		}
		else
		{
			++i;
		}
	}
}

void SplitAsteroid(const AsteroidData& original, std::vector<AsteroidData>& newAsteroids)
{
	if (original.size == AsteroidSize::Large)
	{
		newAsteroids.push_back(CreateAsteroid(original.position, AsteroidSize::Medium));
		newAsteroids.push_back(CreateAsteroid(original.position, AsteroidSize::Medium));
	}
	else if (original.size == AsteroidSize::Medium)
	{
		newAsteroids.push_back(CreateAsteroid(original.position, AsteroidSize::Small));
		newAsteroids.push_back(CreateAsteroid(original.position, AsteroidSize::Small));
	}
}

void ProcessBulletCollisions(AsteroidsData& data)
{
	std::vector<AsteroidData> newAsteroids;

	for (size_t b = 0; b < data.bullets.size();)
	{
		bool hit = false;

		for (size_t a = 0; a < data.asteroids.size(); ++a)
		{
			const auto& asteroid = data.asteroids[a];

			if (IsPointInPolygon(data.bullets[b].position, asteroid.localVertices, asteroid.position, asteroid.angle))
			{
				hit = true;

				if (asteroid.size == AsteroidSize::Large)
				{
					data.state.score += POINTS_PER_LARGE;
				}
				else if (asteroid.size == AsteroidSize::Medium)
				{
					data.state.score += POINTS_PER_MEDIUM;
				}
				else
				{
					data.state.score += POINTS_PER_SMALL;
				}

				SplitAsteroid(asteroid, newAsteroids);

				data.asteroids[a] = data.asteroids.back();
				data.asteroids.pop_back();

				break;
			}
		}

		if (hit)
		{
			data.bullets[b] = data.bullets.back();
			data.bullets.pop_back();
		}
		else
		{
			++b;
		}
	}

	data.asteroids.insert(data.asteroids.end(), newAsteroids.begin(), newAsteroids.end());
}

void ProcessShipCollisions(AsteroidsData& data, float& respawnTimer)
{
	if (!data.ship.isAlive)
	{
		return;
	}

	for (const auto& localVert : data.ship.localVertices)
	{
		Point2f rotated = RotateVector(localVert, data.ship.angle);
		Point2f worldVert = {data.ship.position.x + rotated.x, data.ship.position.y + rotated.y};

		for (size_t a = 0; a < data.asteroids.size(); ++a)
		{
			const auto& asteroid = data.asteroids[a];

			if (IsPointInPolygon(worldVert, asteroid.localVertices, asteroid.position, asteroid.angle))
			{
				data.ship.isAlive = false;
				data.ship.isAccelerating = false;
				data.state.lives -= 1;
				respawnTimer = RESPAWN_DELAY;

				if (data.state.lives <= 0)
				{
					data.state.isGameOver = true;
				}

				std::vector<AsteroidData> newAsteroids;
				SplitAsteroid(asteroid, newAsteroids);

				data.asteroids[a] = data.asteroids.back();
				data.asteroids.pop_back();

				data.asteroids.insert(data.asteroids.end(), newAsteroids.begin(), newAsteroids.end());
				return;
			}
		}
	}
}
} // namespace

AsteroidsModel::AsteroidsModel()
	: m_rotationDirection(RotationDirection::None)
	, m_respawnTimer(0.0f)
	, m_shootCooldownTimer(0.0f)
{
	RestartGame();
}

void AsteroidsModel::UpdatePhysics(float dt)
{
	AssertIsPositive(dt);

	if (m_data.state.isGameOver)
	{
		return;
	}

	if (m_shootCooldownTimer > 0.0f)
	{
		m_shootCooldownTimer -= dt;
	}

	if (!m_data.ship.isAlive)
	{
		m_respawnTimer -= dt;
		if (m_respawnTimer <= 0.0f)
		{
			m_data.ship.position = {AppConfig::WINDOW_WIDTH / 2.0f, AppConfig::WINDOW_HEIGHT / 2.0f};
			m_data.ship.velocity = {0.0f, 0.0f};
			m_data.ship.angle = 0.0f;
			m_data.ship.isAlive = true;
		}
	}

	UpdateShipPhysics(m_data.ship, m_rotationDirection, dt);
	UpdateAsteroidsPhysics(m_data.asteroids, dt);
	UpdateBulletsPhysics(m_data.bullets, dt);

	ProcessBulletCollisions(m_data);
	ProcessShipCollisions(m_data, m_respawnTimer);

	if (m_data.asteroids.empty())
	{
		SpawnInitialAsteroids(m_data.asteroids);
	}

	NotifyObservers();
}

void AsteroidsModel::SetShipAcceleration(bool isAccelerating)
{
	if (m_data.ship.isAlive && !m_data.state.isGameOver)
	{
		m_data.ship.isAccelerating = isAccelerating;
	}
}

void AsteroidsModel::SetShipRotationState(RotationDirection direction)
{
	m_rotationDirection = direction;
}

void AsteroidsModel::Shoot()
{
	if (!m_data.ship.isAlive || m_data.state.isGameOver || m_shootCooldownTimer > 0.0f)
	{
		return;
	}

	m_shootCooldownTimer = SHOOT_COOLDOWN;

	Point2f noseLocal = {15.0f, 0.0f};
	Point2f noseRotated = RotateVector(noseLocal, m_data.ship.angle);
	Point2f spawnPos = {m_data.ship.position.x + noseRotated.x, m_data.ship.position.y + noseRotated.y};

	Point2f direction = RotateVector({1.0f, 0.0f}, m_data.ship.angle);

	BulletData bullet;
	bullet.position = spawnPos;
	bullet.velocity = {direction.x * BULLET_SPEED, direction.y * BULLET_SPEED};
	bullet.timeToLive = BULLET_LIFETIME;

	m_data.bullets.push_back(bullet);
}

void AsteroidsModel::RestartGame()
{
	m_data.ship.position = {AppConfig::WINDOW_WIDTH / 2.0f, AppConfig::WINDOW_HEIGHT / 2.0f};
	m_data.ship.velocity = {0.0f, 0.0f};
	m_data.ship.angle = 0.0f;
	m_data.ship.isAccelerating = false;
	m_data.ship.isAlive = true;
	m_data.ship.localVertices = GenerateShipVertices();

	SpawnInitialAsteroids(m_data.asteroids);
	m_data.bullets.clear();

	m_data.state.score = 0;
	m_data.state.lives = 3;
	m_data.state.isGameOver = false;

	m_rotationDirection = RotationDirection::None;
	m_respawnTimer = 0.0f;
	m_shootCooldownTimer = 0.0f;

	NotifyObservers();
}

const AsteroidsData& AsteroidsModel::GetData() const
{
	return m_data;
}

AsteroidsData AsteroidsModel::GetChangedData() const
{
	return m_data;
}