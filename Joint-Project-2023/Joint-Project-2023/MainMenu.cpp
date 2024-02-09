#include "Menus.h"
#include"MainMenu.h"
#include "Game.h"
#include "GamePlay.h"
#include "Defines.h"
#include <iostream>

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{
}

void MainMenu::initialise(sf::Font& t_font)
{
	m_yOffset = 350;
	m_xOffset = 790;
	m_buttonSpacing = 125;
	m_buttonWidth = 300;
	m_buttonHeight = 103;
	int textDropOffset = 15;
	sf::String m_Texts[] = { "Start Game", "Help", "Exit Game" };

	m_font = t_font;

	if (!m_bgTextr.loadFromFile("ASSETS/IMAGES/bg2.jpg"))
	{
		std::cout << "couldn't load bg texture for help screen" << std::endl;
	}

	m_bgSprite.setTexture(m_bgTextr);
	m_bgSprite.setScale(2, 2);
	m_bgSprite.setPosition(0, -200);

	if (!m_buttonTxtRed.loadFromFile(MENU_RED))
	{
		std::cout << "Can't load button texture";
	}
	if (!m_buttonTxtWhite.loadFromFile(MENU_WHITE))
	{
		std::cout << "Can't load button texture";
	}
	
		m_buttonSprite[0].setTexture(m_buttonTxtWhite);
		m_buttonSprite[0].setPosition(790,350);
		m_buttonSprite[0].setTextureRect(sf::IntRect(0, 0, 600, 206));

		m_buttonSprite[1].setTexture(m_buttonTxtWhite);
		m_buttonSprite[1].setPosition(790, 475);
		m_buttonSprite[1].setTextureRect(sf::IntRect(0, 412, 600, 206));

		m_buttonSprite[2].setTexture(m_buttonTxtWhite);
		m_buttonSprite[2].setPosition(790, 600);
		m_buttonSprite[2].setTextureRect(sf::IntRect(1212, 618, 600, 206));
		//m_buttonSprite[i].setScale(m_buttonWidth / txtSize.x, m_buttonHeight / txtSize.y);
		for (int i = 0; i < m_buttonCount; i++)
		{
			m_buttonSprite[i].setScale(0.5, 0.5);
		}
	m_mainMenuMusic.openFromFile("ASSETS/MUSIC/mainMenuMusic.wav");
	//m_mainMenuMusic.play();
	//m_mainMenuMusic.setVolume(50);
	//m_mainMenuMusic.setLoop(true);
	m_buttonSound.loadFromFile("ASSETS/SOUNDS/buttonSound.wav");

	for (int i = 0; i < 10; i++)
	{
		m_sound[i].setBuffer(m_buttonSound);
		m_sound[i].setVolume(50);
	}
}

void MainMenu::update(sf::Time& t_deltaTime, sf::Window& t_window)
{
	sf::Vector2i mouseLocation;
	mouseLocation = sf::Mouse::getPosition(t_window);
	for (int i = 0; i < m_buttonCount; i++)
	{
		m_text[i].setFillColor(sf::Color::White);
	}
	if (mouseLocation.x > m_xOffset && mouseLocation.x < m_xOffset + m_buttonWidth)
	{
		if (mouseLocation.y > m_yOffset && mouseLocation.y < m_yOffset + m_buttonHeight)
		{
			if (m_highlightedMenu != MenuHighlighted::Play)
			{
				m_sound[currentSound].play();
			}
			currentSound++;
			if (currentSound >= 10)
			{
				currentSound = 0;
			}
			m_highlightedMenu = MenuHighlighted::Play;

			m_buttonSprite[0].setTexture(m_buttonTxtRed);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				
				m_mainMenuMusic.stop();
				Menus::m_currentGameState = GameStates::GamePlay;
			}
		}
		else
		{
			m_buttonSprite[0].setTexture(m_buttonTxtWhite);
		}
		if (mouseLocation.y > m_yOffset + m_buttonSpacing && mouseLocation.y < m_yOffset + m_buttonHeight + m_buttonSpacing)
		{
			if (m_highlightedMenu != MenuHighlighted::Help)
			{
				m_sound[currentSound].play();
			}
			currentSound++;
			if (currentSound >= 10)
			{
				currentSound = 0;
			}
			m_highlightedMenu = MenuHighlighted::Help;

			m_buttonSprite[1].setTexture(m_buttonTxtRed);

			m_text[1].setFillColor(sf::Color{ 75,75,75,255 });
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				
				Menus::m_currentGameState = GameStates::Help;
			}
		}
		else
		{
			m_buttonSprite[1].setTexture(m_buttonTxtWhite);
		}
		if (mouseLocation.y > m_yOffset + (m_buttonSpacing * 2) && mouseLocation.y < m_yOffset + m_buttonHeight + (m_buttonSpacing * 2))
		{
			if (m_highlightedMenu != MenuHighlighted::Exit)
			{
				m_sound[currentSound].play();
			}
			currentSound++;
			if (currentSound >= 10)
			{
				currentSound = 0;
			}
			m_buttonSprite[2].setTexture(m_buttonTxtRed);

			m_highlightedMenu = MenuHighlighted::Exit;

			m_text[2].setFillColor(sf::Color{ 75,75,75,255 });
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				t_window.close();
			}
		}
		else
		{
			m_buttonSprite[2].setTexture(m_buttonTxtWhite);
		}
		
	}
	else
	{
		for (int i = 0; i < m_buttonCount; i++)
		{
			m_buttonSprite[i].setTexture(m_buttonTxtWhite);
		}
	}
}

void MainMenu::render(sf::RenderWindow& t_window)
{

	t_window.draw(m_bgSprite);
	t_window.draw(m_buttonSprite[0]);
	t_window.draw(m_buttonSprite[1]);
	t_window.draw(m_buttonSprite[2]);
	for (int i = 0; i < m_buttonCount; i++)
	{
		t_window.draw(m_buttonSprite[i]);
		t_window.draw(m_text[i]);
	}
}