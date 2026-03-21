#include "AsteroidsView.h"
#include "src/controller/game/AsteroidsController.h"
#include "src/system/AppConfig.h"
#include <cmath>
#include <string>

namespace
{
constexpr uint32_t UI_FONT_ID = 1;

Color GetTextColor(bool isDark)
{
	return isDark ? AppConfig::DarkTheme::PRIMARY_TEXT : AppConfig::LightTheme::PRIMARY_TEXT;
}

std::vector<Point2f> TransformVertices(const std::vector<Point2f>& localVertices, const Point2f& position, float angle)
{
	std::vector<Point2f> worldVertices;
	worldVertices.reserve(localVertices.size());

	float cosA = std::cos(angle);
	float sinA = std::sin(angle);

	for (const auto& local : localVertices)
	{
		float rx = local.x * cosA - local.y * sinA;
		float ry = local.x * sinA + local.y * cosA;
		worldVertices.push_back({position.x + rx, position.y + ry});
	}

	return worldVertices;
}

void RenderFlame(IRenderer& renderer, const Point2f& position, float angle)
{
	std::vector<Point2f> flameLocal = {
		{-10.0f, -6.0f},
		{-10.0f, 6.0f},
		{-28.0f, 0.0f}};

	auto flameWorld = TransformVertices(flameLocal, position, angle);

	RenderStyle flameStyle;
	flameStyle.fillColor = Color::FromRGBA(255, 80, 0, 255);

	renderer.DrawPolygon(flameWorld, flameStyle);
}

void RenderShip(IRenderer& renderer, const ShipData& ship)
{
	if (!ship.isAlive)
	{
		return;
	}

	if (ship.isAccelerating)
	{
		RenderFlame(renderer, ship.position, ship.angle);
	}

	auto worldVerts = TransformVertices(ship.localVertices, ship.position, ship.angle);

	RenderStyle style;
	style.fillColor = Color::FromRGBA(40, 160, 255, 255);
	renderer.DrawPolygon(worldVerts, style);

	worldVerts.push_back(worldVerts.front());
	renderer.DrawPolyline(worldVerts, Color::FromRGBA(120, 200, 255, 255), 2.0f);
}

void RenderAsteroids(IRenderer& renderer, const std::vector<AsteroidData>& asteroids, bool isDark)
{
	RenderStyle style;
	style.fillColor = isDark ? Color::FromRGBA(60, 60, 60, 255) : Color::FromRGBA(160, 160, 160, 255);
	Color outlineColor = isDark ? Color::FromRGBA(100, 100, 100, 255) : Color::FromRGBA(100, 100, 100, 255);

	for (const auto& asteroid : asteroids)
	{
		auto worldVerts = TransformVertices(asteroid.localVertices, asteroid.position, asteroid.angle);

		renderer.DrawPolygon(worldVerts, style);

		worldVerts.push_back(worldVerts.front());
		renderer.DrawPolyline(worldVerts, outlineColor, 2.0f);
	}
}

void RenderBullets(IRenderer& renderer, const std::vector<BulletData>& bullets)
{
	RenderStyle style;
	style.fillColor = Color::FromRGBA(255, 255, 100, 255);

	for (const auto& bullet : bullets)
	{
		renderer.DrawEllipse(bullet.position, {3.0f, 3.0f}, style);
	}
}

void RenderUI(IRenderer& renderer, const GameStateData& state, bool isDark)
{
	Color textColor = GetTextColor(isDark);

	std::string scoreText = "SCORE: " + std::to_string(state.score);
	renderer.DrawTextData({20.0f, 30.0f}, scoreText, UI_FONT_ID, textColor);

	std::string livesText = "LIVES: " + std::to_string(state.lives);
	renderer.DrawTextData({AppConfig::WINDOW_WIDTH - 120.0f, 30.0f}, livesText, UI_FONT_ID, textColor);

	if (state.isGameOver)
	{
		std::string gameOverText = "GAME OVER";
		float textX = AppConfig::WINDOW_WIDTH / 2.0f;
		float textY = AppConfig::WINDOW_HEIGHT / 2.0f;
		renderer.DrawTextData({textX, textY}, gameOverText, UI_FONT_ID, textColor);

		std::string hintText = "Press ENTER to restart";
		renderer.DrawTextData({textX, textY + 30.0f}, hintText, UI_FONT_ID, textColor);
	}
}
} // namespace

AsteroidsView::AsteroidsView(
	std::shared_ptr<AsteroidsModel> model,
	std::shared_ptr<AsteroidsController> controller,
	std::shared_ptr<ThemeModel> themeModel)
	: m_controller(std::move(controller))
{
	if (model)
	{
		Update(model->GetData(), nullptr);
	}

	if (themeModel)
	{
		Update(themeModel->GetData(), nullptr);
	}
}

void AsteroidsView::HandleEvent(const Event& event)
{
	m_controller->HandleEvent(event);
}

void AsteroidsView::Render(IRenderer& renderer) const
{
	RenderBullets(renderer, m_data.bullets);
	RenderAsteroids(renderer, m_data.asteroids, m_themeData.isDark);
	RenderShip(renderer, m_data.ship);
	RenderUI(renderer, m_data.state, m_themeData.isDark);
}

void AsteroidsView::Update(const AsteroidsData& data, IObservable<AsteroidsData>*)
{
	m_data = data;
}

void AsteroidsView::Update(const ThemeData& data, IObservable<ThemeData>*)
{
	m_themeData = data;
}