#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class MenuHighlighted
{
	None,
	Play,
	Help,
	Exit
};

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void initialise(sf::Font& t_font);
	void update(sf::Time& t_deltaTime, sf::Window& t_window);
	void render(sf::RenderWindow& t_window);

private:

	static const int m_buttonCount = 3;
	sf::Texture m_buttonTxtWhite;
	sf::Texture m_buttonTxtRed;

	sf::Sprite m_buttonSprite[m_buttonCount];
	sf::Text m_text[m_buttonCount];
	sf::Font m_font;

	sf::Texture m_bgTextr;
	sf::Sprite m_bgSprite;

	float m_yOffset{ 0.0f };
	float m_xOffset{ 0.0f };
	float m_buttonSpacing{ 0.0f };
	float m_buttonWidth{ 0.0f };
	float m_buttonHeight{ 0.0f };

	sf::SoundBuffer m_buffer; // Sound buffer
	sf::Music m_mainMenuMusic;

	int currentSound = 0;
	sf::Sound m_sound[10];
	sf::SoundBuffer m_buttonSound;

	MenuHighlighted m_highlightedMenu = MenuHighlighted::None;
};
