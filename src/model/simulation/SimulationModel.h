#pragma once
#include "SFML/System/Vector2.hpp"
#include "src/system/Observer.h"
#include <vector>

struct BallData
{
	sf::Vector2f position;
	sf::Vector2f velocity;
	float radius;
};

struct RectObstacleData
{
	sf::Vector2f center;
	sf::Vector2f size;
	float rotationAngle;
};

struct CannonData
{
	sf::Vector2f position;
	sf::Vector2f targetDirection;
};

struct SimulationData
{
	std::vector<BallData> balls;
	std::vector<RectObstacleData> obstacles;
	CannonData cannon;
};

class SimulationModel final : public CObservable<SimulationData>
{
public:
	SimulationModel();

	void Update(float dt);
	void SetTarget(float x, float y);
	void Shoot();
	void IncreaseTimeScale();
	void DecreaseTimeScale();

	const SimulationData& GetData() const;

protected:
	SimulationData GetChangedData() const override;

private:
	void ApplyGravity(float dt);
	void MoveBalls(float dt);
	void ResolveCollisions();
	void RemoveFallenBalls();

	SimulationData m_data;
	float m_timeScale;
};