#include "BoardView.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "src/model/ChessGrid.h"

namespace
{
constexpr sf::Color COLOR_LIGHT(222, 227, 230);
constexpr sf::Color COLOR_DARK(140, 162, 173);
} // namespace

void BoardView::HandleEvent(const sf::Event&, const sf::RenderWindow&)
{
}

void BoardView::Render(sf::RenderWindow& window) const
{
	sf::RectangleShape cell({
		static_cast<float>(ChessGrid::CELL_SIZE),
		static_cast<float>(ChessGrid::CELL_SIZE)
	});

	for (int y = 0; y < ChessGrid::BOARD_SIZE; ++y)
	{
		for (int x = 0; x < ChessGrid::BOARD_SIZE; ++x)
		{
			sf::Vector2f pos = ChessGrid::GridToPixels({x, y});
			cell.setPosition(pos);

			bool isWhite = (x + y) % 2 == 0;
			cell.setFillColor(isWhite ? COLOR_LIGHT : COLOR_DARK);

			window.draw(cell);
		}
	}
}