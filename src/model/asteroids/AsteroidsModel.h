#pragma once
#include "src/core/types/Point.h"
#include "src/system/Observer.h"

enum class ShipState
{
	Alive,
	Destroyed
};

struct ShipData
{
	Point2f position;
	Point2f velocity;
	float angle;
	bool isThrusting;
	ShipState state;
};

struct AsteroidsData
{
	ShipData ship;
	int score;
	int lives;
	bool isGameOver;
};

class AsteroidsModel final : public CObservable<AsteroidsData>
{
public:
	AsteroidsModel();

	void Update(float dt);
	void SetThrust(bool isThrusting);
	void SetRotation(float direction);
	void Shoot();

	const AsteroidsData& GetData() const;

protected:
	AsteroidsData GetChangedData() const override;

private:
	void UpdateShipPhysics(float dt);
	static void WrapCoordinates(Point2f& position);

	AsteroidsData m_data;
	float m_rotationDirection;
};