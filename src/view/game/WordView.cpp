#include "WordView.h"
#include "src/system/AppConfig.h"

WordView::WordView(const WordData& initialData)
	: m_descriptionText(m_font)
	, m_hintText(m_font)
	, m_wordText(m_font)
{
	AppConfig::LoadDefaultFont(m_font);

	m_descriptionText.setCharacterSize(18);
	m_descriptionText.setFillColor(AppConfig::DarkTheme::PRIMARY_TEXT);

	m_hintText.setCharacterSize(14);
	m_hintText.setFillColor(sf::Color(150, 150, 150));

	m_wordText.setCharacterSize(36);
	m_wordText.setFillColor(AppConfig::DarkTheme::PRIMARY_TEXT);
	m_wordText.setStyle(sf::Text::Bold);

	Update(initialData, nullptr);
	UpdateLayout(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT);
}

void WordView::HandleEvent(const sf::Event& event, const sf::RenderWindow&)
{
	if (const auto* resized = event.getIf<sf::Event::Resized>())
	{
		UpdateLayout(resized->size.x, resized->size.y);
	}
}

void WordView::Render(sf::RenderWindow& window) const
{
	window.draw(m_descriptionText);
	window.draw(m_hintText);
	window.draw(m_wordText);
}

void WordView::Update(const WordData& data, IObservable<WordData>*)
{
	m_descriptionText.setString(data.riddle.description);
	m_hintText.setString("Подсказка: " + data.riddle.hint);
	m_wordText.setString(AddSpaces(data.maskedWord));

	m_descriptionText.setString(AppConfig::ToSfString(data.riddle.description));
	m_hintText.setString(AppConfig::ToSfString("Подсказка: " + data.riddle.hint));
	m_wordText.setString(AppConfig::ToSfString(AddSpaces(data.maskedWord)));

	UpdateLayout(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT);
}

void WordView::UpdateLayout(unsigned int windowWidth, unsigned int windowHeight)
{
	sf::Vector2f center = AppConfig::GetWindowCenter(windowWidth, windowHeight);

	sf::FloatRect descBounds = m_descriptionText.getLocalBounds();
	m_descriptionText.setPosition({
		center.x - descBounds.size.x / 2.0f,
		center.y - 80.0f
	});

	sf::FloatRect hintBounds = m_hintText.getLocalBounds();
	m_hintText.setPosition({
		center.x - hintBounds.size.x / 2.0f,
		center.y - 50.0f
	});

	sf::FloatRect wordBounds = m_wordText.getLocalBounds();
	m_wordText.setPosition({
		center.x - wordBounds.size.x / 2.0f,
		center.y - 10.0f
	});
}

std::string WordView::AddSpaces(const std::string& word)
{
	std::string spacedWord;
	for (char ch : word)
	{
		spacedWord += ch;
		spacedWord += ' ';
	}
	return spacedWord;
}