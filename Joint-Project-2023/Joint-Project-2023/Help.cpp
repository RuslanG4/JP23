#include "Help.h"
#include"Menus.h"
Help::Help()
{
}

Help::~Help()
{
}

void Help::initialise(sf::Font& t_font)
{
	m_yOffset = 25;
	m_xOffset = 25;
	m_buttonWidth = 300;
	m_buttonHeight = 103;

	m_font = t_font;
	m_text.setFont(m_font);
	m_text.setCharacterSize(52);
	m_text.setString("Main Menu");
	m_text.setPosition(sf::Vector2f(m_xOffset, m_yOffset));

	if (!m_bgTextr.loadFromFile("ASSETS/IMAGES/bg2.jpg"))
	{
		std::cout << "couldn't load bg texture for help screen" << std::endl;
	}

	m_bgSprite.setTexture(m_bgTextr);
	m_bgSprite.setScale(2, 2);
	m_bgSprite.setPosition(0, -200);

	if (!m_controlsTextr.loadFromFile("ASSETS/IMAGES/controls.png"))
	{
		std::cout << "couldn't load bg texture for help screen" << std::endl;
	}

	m_controlsSprite.setTexture(m_controlsTextr);
	//m_controlsSprite.setScale(2, 2);
	//m_controlsSprite.setPosition(0, -200);

	if (!m_buttonTxtRed.loadFromFile(MENU_RED))
	{
		std::cout << "Can't load button texture";
	}
	if (!m_buttonTxtWhite.loadFromFile(MENU_WHITE))
	{
		std::cout << "Can't load button texture";
	}

	m_buttonSprite.setTexture(m_buttonTxtWhite);
	m_buttonSprite.setPosition(25, 25);
	m_buttonSprite.setTextureRect(sf::IntRect(1212, 206, 600, 206));

	m_buttonSprite.setScale(0.5, 0.5);

	m_buttonSound.loadFromFile("ASSETS/SOUNDS/buttonSound.wav");
	m_sound.setBuffer(m_buttonSound);
	m_sound.setVolume(50);
}

void Help::update(sf::Time& t_deltaTime, sf::Window& t_window)
{
	sf::Vector2i mouseLocation;
	mouseLocation = sf::Mouse::getPosition(t_window);

	m_text.setFillColor(sf::Color::White);
	
	if (mouseLocation.x >m_xOffset && mouseLocation.x <m_xOffset +m_buttonWidth)
	{
		if (mouseLocation.y > m_yOffset && mouseLocation.y < m_yOffset+ m_buttonHeight)
		{

			m_sound.play();

			m_buttonSprite.setTexture(m_buttonTxtRed);

			m_text.setFillColor(sf::Color{ 75,75,75,255 });
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{

				m_mainMenuMusic.stop();
				Menus::m_currentGameState = GameStates::MainMenu;
			}
		}
		else
		{
			m_buttonSprite.setTexture(m_buttonTxtWhite);
		}
	}
	else
	{
		m_buttonSprite.setTexture(m_buttonTxtWhite);
	}
}

void Help::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_bgSprite);
	//t_window.draw(m_text);
	t_window.draw(m_buttonSprite);
	t_window.draw(m_controlsSprite);
}
