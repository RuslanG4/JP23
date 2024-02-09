#pragma once
#include <SFML/Graphics.hpp>
#include "GamePlay.h"
#include "Help.h"
#include "License.h"
#include "MainMenu.h"
#include"ScoreScreen.h"
#include"Defines.h"

enum class GameStates
{
	License,
	MainMenu,
	Help,
	GamePlay,
	Scoring
};


class Menus
{
public:
	Menus(sf::RenderWindow& t_window);
	void update(sf::Time& t_deltaTime);
	void render(sf::RenderWindow& t_window);
	void processInput(sf::Event& t_event);
	void initialise(sf::Font &font);

	sf::Font myfont;
	sf::RenderWindow& window;
	sf::Clock clock;
	static GameStates m_currentGameState;

	

private:
	
	sf::RenderWindow m_window;
	
	GamePlay* m_gamePlayScreen;
	Help m_helpScreen;
	License m_licenseScreen;
	MainMenu m_mainMenusScreen;
	ScoreScreen* m_scoreScreen;

};

