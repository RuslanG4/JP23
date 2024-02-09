#pragma once
#include <SFML/Graphics.hpp>
#include"Defines.h"
#include "Globals.h"
#include <string>
#include <iostream>

class ScoreIncreaseText : public sf::Text
{
public:
	ScoreIncreaseText() = default;

	bool update(sf::Vector2f t_pos)
	{
		offset += velocity;
		this->setPosition(t_pos + offset);
		if (currentColor.a <= colorDecrement)
		{
			onScreen = false;			
		}
		currentColor.a -= colorDecrement;
		
		this->setFillColor(currentColor);
		return onScreen;
	}
	
	sf::Vector2f velocity{10,0};
	sf::Vector2f offset{0,0};
	sf::Color currentColor;
	int colorDecrement = 10;
	bool onScreen = true;
};

class HUD
{
public:
	void update(sf::Time& t_deltaTime);
	void render(sf::RenderWindow& t_window);
	void initialise(sf::Texture& texture, sf::Texture& healthBarTexture);

	void idleAnimation();
	void followCamera(sf::Vector2f _pos);
	void setGunHudTexture(sf::Texture& text);
	void setAmmoHudNumber(int t_ammoCount);
	void adjustHealthBar(int t_health);
	void updateScore(float t_score);
	void updateIncrease(int t_scoreIncrease);

	int getScore() { return score; };

private:
	sf::RectangleShape iconBG[3];



	//HEALTH BAR
	sf::RectangleShape healthRect;
	sf::Sprite healthBar;
	sf::Vector2f healthBarRectSize;
	sf::Vector2f maxHealthBarRectSize;
	sf::Vector2f healthRectOffest;

	sf::Text text;
	sf::Font font;

	//Score
	sf::Text m_scoreText;
	sf::Text m_scoreIncreaseText;
	std::vector<ScoreIncreaseText> m_scoreIncreaseTexts;

	sf::Sprite playerSprite;
	sf::Sprite gunSprite;

	
	sf::Texture healthBarTexture;
	sf::Texture playerTexture;

	int idleTime = 0;
	int idleFrame = 0;

	int ammoCount = 0;

	int score = 0;
};

