#include "ScoreView.h"
#include "src/system/AppConfig.h"

ScoreView::ScoreView(const ScoreData& initialData)
	: m_scoreText(m_font)
{
	AppConfig::LoadDefaultFont(m_font);

	m_scoreText.setCharacterSize(24);
	m_scoreText.setFillColor(AppConfig::DarkTheme::PRIMARY_TEXT);
	m_scoreText.setStyle(sf::Text::Bold);

	Update(initialData, nullptr);
	UpdateLayout(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT);
}

void ScoreView::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	if (const auto* resized = event.getIf<sf::Event::Resized>())
	{
		UpdateLayout(resized->size.x, resized->size.y);
	}
}

void ScoreView::Render(sf::RenderWindow& window) const
{
	window.draw(m_scoreText);
}

void ScoreView::Update(const ScoreData& data, IObservable<ScoreData>*)
{
	std::string text = "Счет: " + std::to_string(data.score) +
					   " | Ошибки: " + std::to_string(data.mistakes) + "/" + std::to_string(data.maxMistakes);

	m_scoreText.setString(AppConfig::ToSfString(text));
	UpdateLayout(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT);
}

void ScoreView::UpdateLayout(unsigned int windowWidth, unsigned int windowHeight)
{
	sf::Vector2f center = AppConfig::GetWindowCenter(windowWidth, windowHeight);
	sf::FloatRect bounds = m_scoreText.getLocalBounds();

	m_scoreText.setPosition({
		center.x - bounds.size.x / 2.0f,
		20.0f
	});
}