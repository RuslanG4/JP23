#include "Menus.h"
#include "License.h"




void License::render(sf::RenderWindow& window)
{
	window.draw(text);
}

void License::initialise(sf::Font& t_font)
{
	font = t_font;
	text.setFont(font);
	text.setCharacterSize(52);
	text.setFillColor(sf::Color::Red);
	text.setString("Made by Richie \n  and Ruslan");
	text.setPosition(SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2-100);
}

void License::update()
{
	if (m_clock.getElapsedTime() > licenseTime)
	{
		Menus::m_currentGameState = GameStates::MainMenu;
	}
}
