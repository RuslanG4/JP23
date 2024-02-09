#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<iostream>

class Help
{
public:
	Help();
	~Help();

	void initialise(sf::Font& t_font);
	void update(sf::Time& t_deltaTime, sf::Window& t_window);
	void render(sf::RenderWindow& t_window);

protected:

	sf::Texture m_buttonTxtWhite;
	sf::Texture m_buttonTxtRed;

	sf::Sprite m_buttonSprite;

	sf::Text m_text;
	sf::Font m_font;

	sf::Texture m_bgTextr;
	sf::Sprite m_bgSprite;

	sf::Texture m_controlsTextr;
	sf::Sprite m_controlsSprite;

	float m_yOffset{ 0.0f };
	float m_xOffset{ 0.0f };
	float m_buttonSpacing{ 0.0f };
	float m_buttonWidth{ 0.0f };
	float m_buttonHeight{ 0.0f };

	sf::SoundBuffer m_buffer; // Sound buffer
	sf::Music m_mainMenuMusic;

	sf::Sound m_sound;
	sf::SoundBuffer m_buttonSound;
};

