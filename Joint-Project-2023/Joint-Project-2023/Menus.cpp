#include "Menus.h"

GameStates Menus::m_currentGameState = GameStates::License;

Menus::Menus(sf::RenderWindow& t_window) : window(t_window)
{
	m_gamePlayScreen = new GamePlay;
	m_scoreScreen = new ScoreScreen;
}

void Menus::update(sf::Time& t_deltaTime)
{

	if (m_scoreScreen->reset)
	{
		delete m_gamePlayScreen;
		m_gamePlayScreen = new GamePlay;

		delete m_scoreScreen;
		m_scoreScreen = new ScoreScreen;
		m_scoreScreen->initialise(myfont);
	}
	switch (m_currentGameState)
	{
	case GameStates::License:
		m_licenseScreen.update();
		break;
	case GameStates::MainMenu:
		m_mainMenusScreen.update(t_deltaTime, window);
		break;
	case GameStates::Help:
		m_helpScreen.update(t_deltaTime, window);
		break;
	case GameStates::GamePlay:
		m_gamePlayScreen->update(t_deltaTime);
		m_scoreScreen->setVariables(m_gamePlayScreen->getScore(), m_gamePlayScreen->getEnemiesKilled());
		break;
	case GameStates::Scoring:
		m_scoreScreen->update(t_deltaTime, window);
		break;
	default:
		break;
	}
}

void Menus::render(sf::RenderWindow& t_window)
{
	switch (m_currentGameState)
	{
	case GameStates::License:
		m_licenseScreen.render(window);
		break;
	case GameStates::MainMenu:
		m_mainMenusScreen.render(window);
		break;
	case GameStates::Help:
		m_helpScreen.render(window);
		break;
	case GameStates::GamePlay:
		m_gamePlayScreen->render(window);
		break;
	case GameStates::Scoring:
		m_scoreScreen->render(window);
		break;
	default:
		break;
	}
}


void Menus::processInput(sf::Event& t_event)
{
	switch (m_currentGameState)
	{
	case GameStates::License:

		break;
	case GameStates::MainMenu:

		break;
	case GameStates::Help:

		break;
	case GameStates::GamePlay:
		m_gamePlayScreen->processInput(t_event);
		break;
	case GameStates::Scoring:

		break;
	default:
		break;
	}
}

void Menus::initialise(sf::Font& font)
{
	//Load here and call other screen initilaise methods
	//m_gamePlayScreen.initialise();
	m_licenseScreen.initialise(font);
	m_mainMenusScreen.initialise(font);
	m_helpScreen.initialise(font);
	m_scoreScreen->initialise(font);

	myfont = font;
}

