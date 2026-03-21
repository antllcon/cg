#pragma once
#include "src/core/types/Point.h"
#include "src/system/Observer.h"
#include <vector>

enum class RotationDirection
{
	None,
	Left,
	Right
};

enum class AsteroidSize
{
	Large,
	Medium,
	Small
};

struct ShipData
{
	Point2f position{0.0f, 0.0f};
	Point2f velocity{0.0f, 0.0f};
	float angle{0.0f};
	bool isAccelerating{false};
	bool isAlive{true};
	std::vector<Point2f> localVertices;
};

struct AsteroidData
{
	Point2f position{0.0f, 0.0f};
	Point2f velocity{0.0f, 0.0f};
	float angle{0.0f};
	float rotationSpeed{0.0f};
	AsteroidSize size{AsteroidSize::Large};
	std::vector<Point2f> localVertices;
};

struct BulletData
{
	Point2f position{0.0f, 0.0f};
	Point2f velocity{0.0f, 0.0f};
	float timeToLive{0.0f};
};

struct GameStateData
{
	int score{0};
	int lives{3};
	bool isGameOver{false};
};

struct AsteroidsData
{
	ShipData ship;
	std::vector<AsteroidData> asteroids;
	std::vector<BulletData> bullets;
	GameStateData state;
};

class AsteroidsModel final : public CObservable<AsteroidsData>
{
public:
	AsteroidsModel();

	void UpdatePhysics(float dt);
	void SetShipAcceleration(bool isAccelerating);
	void SetShipRotationState(RotationDirection direction);
	void Shoot();
	void RestartGame();

	const AsteroidsData& GetData() const;

protected:
	AsteroidsData GetChangedData() const override;

private:
	AsteroidsData m_data;
	RotationDirection m_rotationDirection;
	float m_respawnTimer;
	float m_shootCooldownTimer;
};