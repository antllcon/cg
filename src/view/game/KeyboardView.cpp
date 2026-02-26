#include "KeyboardView.h"
#include "src/system/AppConfig.h"
#include <ranges>
#include <stdexcept>

namespace
{
	void AssertPointerNotNull(const void* ptr)
	{
		if (ptr == nullptr)
		{
			throw std::runtime_error("Указатель не должен быть нулевым");
		}
	}

	bool IsMouseOverShape(const sf::RectangleShape& shape, const sf::RenderWindow& window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		return shape.getGlobalBounds().contains(worldPos);
	}
}

KeyboardView::KeyboardView(std::shared_ptr<KeyboardController> controller, const KeyboardData& initialData)
	: m_controller(controller)
{
	AssertPointerNotNull(m_controller.get());
	AppConfig::LoadDefaultFont(m_font);

	InitKeys();
	Update(initialData, nullptr);
}

KeyboardView::~KeyboardView()
{
}

void KeyboardView::HandleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
	if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mouseButton->button == sf::Mouse::Button::Left)
		{
			HandleMouseClick(window);
		}
	}
	else if (const auto* resized = event.getIf<sf::Event::Resized>())
	{
		UpdateLayout(resized->size.x, resized->size.y);
	}
}

void KeyboardView::Render(sf::RenderWindow& window) const
{
	for (const auto& visuals : m_keys | std::views::values)
	{
		window.draw(visuals.shape);
		window.draw(visuals.text);
	}
}

void KeyboardView::Update(const KeyboardData& data, IObservable<KeyboardData>*)
{
	for (const auto& [letter, status] : data.keys)
	{
		ApplyLetterState(letter, status);
	}
}

void KeyboardView::InitKeys()
{
	for (char character = 'A'; character <= 'Z'; ++character)
	{
		sf::RectangleShape shape(m_keySize);
		shape.setFillColor(sf::Color(100, 100, 100));

		sf::Text text(m_font);
		text.setString(std::string(1, character));
		text.setCharacterSize(AppConfig::FONT_SIZE);
		text.setFillColor(AppConfig::DarkTheme::PRIMARY_TEXT);

		m_keys.insert({character, KeyVisuals{std::move(shape), std::move(text), true}});
	}

	UpdateLayout(AppConfig::WINDOW_WIDTH, AppConfig::WINDOW_HEIGHT);
}

void KeyboardView::UpdateLayout(unsigned int windowWidth, unsigned int windowHeight)
{
	for (auto& [character, visuals] : m_keys)
	{
		sf::Vector2f pos = CalculateKeyPosition(character, windowWidth, windowHeight);

		visuals.shape.setSize(m_keySize);
		visuals.shape.setPosition(pos);

		sf::FloatRect bounds = visuals.text.getLocalBounds();
		visuals.text.setPosition({
			pos.x + (m_keySize.x - bounds.size.x) / 2.0f - bounds.position.x,
			pos.y + (m_keySize.y - bounds.size.y) / 2.0f - bounds.position.y
		});
	}
}

sf::Vector2f KeyboardView::CalculateKeyPosition(char letter, unsigned int windowWidth, unsigned int windowHeight) const
{
	int index = letter - 'A';
	int row = index / m_keysPerRow;
	int col = index % m_keysPerRow;

	int totalRows = (26 + m_keysPerRow - 1) / m_keysPerRow;
	int keysInCurrentRow = (row == totalRows - 1) ? (26 % m_keysPerRow == 0 ? m_keysPerRow : 26 % m_keysPerRow) : m_keysPerRow;

	float rowWidth = (keysInCurrentRow * m_keySize.x) + ((keysInCurrentRow - 1) * m_keyGap);
	float centerX = AppConfig::GetWindowCenter(windowWidth, windowHeight).x;
	float startX = centerX - (rowWidth / 2.0f);
	float totalHeight = (totalRows * m_keySize.y) + ((totalRows - 1) * m_keyGap);
	float startY = windowHeight - m_bottomMargin - totalHeight;

	float x = startX + col * (m_keySize.x + m_keyGap);
	float y = startY + row * (m_keySize.y + m_keyGap);

	return {x, y};
}

void KeyboardView::HandleMouseClick(const sf::RenderWindow& window)
{
	for (const auto& [letter, visuals] : m_keys)
	{
		if (visuals.isEnabled && IsMouseOverShape(visuals.shape, window))
		{
			ProcessKeyClick(letter);
			return;
		}
	}
}

void KeyboardView::ProcessKeyClick(char letter)
{
	m_controller->OnLetterClicked(letter);
}

void KeyboardView::ApplyLetterState(char letter, LetterStatus status)
{
	auto& visuals = m_keys.at(letter);
	if (status == LetterStatus::AVAILABLE)
	{
		visuals.isEnabled = true;
		visuals.shape.setFillColor(sf::Color(100, 100, 100));
	}
	else if (status == LetterStatus::GUESSED_CORRECTLY)
	{
		visuals.isEnabled = false;
		visuals.shape.setFillColor(sf::Color(46, 204, 113));
	}
	else
	{
		visuals.isEnabled = false;
		visuals.shape.setFillColor(sf::Color(231, 76, 60));
	}
}