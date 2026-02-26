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

	void AssertIsFontLoaded(bool isLoaded)
	{
		if (!isLoaded)
		{
			throw std::runtime_error("Не удалось загрузить шрифт для клавиатуры");
		}
	}

	sf::Vector2f CalculateKeyPosition(char letter)
	{
		int index = letter - 'A';
		int col = index % 7;
		int row = index / 7;

		float x = 32.0f + col * 48.0f;
		float y = 200.0f + row * 48.0f;

		return {x, y};
	}

	bool IsMouseOverShape(const sf::RectangleShape& shape, const sf::RenderWindow& window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

		return shape.getGlobalBounds().contains(worldPos);
	}
}

// Изменён конструктор
KeyboardView::KeyboardView(std::shared_ptr<KeyboardController> controller, const KeyboardData& initialData)
	: m_controller(controller)
{
	AssertPointerNotNull(m_controller.get());
	AssertIsFontLoaded(m_font.openFromFile(AppConfig::FONT_PATH));

	InitKeys();

	// Вызываем Update с начальными данными
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
	sf::Vector2f keySize(40.0f, 40.0f);

	for (char character = 'A'; character <= 'Z'; ++character)
	{
		sf::Vector2f pos = CalculateKeyPosition(character);

		sf::RectangleShape shape(keySize);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color(100, 100, 100));
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(2.0f);

		sf::Text text(m_font);
		text.setString(std::string(1, character));
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);

		sf::FloatRect bounds = text.getLocalBounds();
		text.setPosition({
			pos.x + (keySize.x - bounds.size.x) / 2.0f - bounds.position.x,
			pos.y + (keySize.y - bounds.size.y) / 2.0f - bounds.position.y
		});

		m_keys.insert({character, KeyVisuals{std::move(shape), std::move(text), true}});
	}
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