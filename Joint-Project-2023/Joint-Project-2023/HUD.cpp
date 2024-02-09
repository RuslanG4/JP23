#include "HUD.h"
#include "Globals.h"
#include <iostream>
#include <iomanip>
#include <sstream>

void HUD::update(sf::Time& t_deltaTime)
{
	idleAnimation();
	healthRect.setSize(healthBarRectSize);
}

void HUD::render(sf::RenderWindow& t_window)
{
	/*t_window.draw(overlay);*/
	for (int i = 0; i < 3; i++)
	{
		t_window.draw(iconBG[i]);
	}
	t_window.draw(playerSprite);
	t_window.draw(gunSprite);
	t_window.draw(healthRect);

	t_window.draw(text);
	t_window.draw(m_scoreText);
	for (int i = 0; i < m_scoreIncreaseTexts.size(); i++)
	{
		if (m_scoreIncreaseTexts[i].onScreen == true)
		{
			t_window.draw(m_scoreIncreaseTexts[i]);
		}
	}
	
}

void HUD::initialise(sf::Texture& texture, sf::Texture& t_healthBarTexture)
{
	playerTexture = texture;
	healthBarTexture = t_healthBarTexture;

	playerSprite.setTexture(texture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	playerSprite.setOrigin(16, 16);
	playerSprite.setScale(2, 2);

	healthBar.setTexture(t_healthBarTexture);
	sf::Vector2u textureSize = t_healthBarTexture.getSize();
	sf::Vector2f scale{ 0.2,0.5 };
	textureSize.x *= scale.x;
	textureSize.y *= scale.y;
	healthBar.setScale(scale);

	healthRect.setFillColor(sf::Color::Red);
	healthBarRectSize = static_cast<sf::Vector2f>(textureSize);
	healthBarRectSize.x = 20;
	healthBarRectSize.y = -100;
	maxHealthBarRectSize = healthBarRectSize;
	healthRect.setSize(healthBarRectSize);
	healthRect.setOrigin(10, 50);
	
	gunSprite.setScale(0.25, 0.25);
	gunSprite.setOrigin(100, 100);

	for (int i = 0; i < 3; i++)
	{
		iconBG[i].setFillColor(GRAY);
		iconBG[i].setOutlineColor(SWAMP);
		iconBG[i].setOutlineThickness(3);
	}

	iconBG[0].setSize(sf::Vector2f(100, 100));
	iconBG[0].setOrigin(50, 50);

	iconBG[1].setSize(sf::Vector2f(100, 50));
	iconBG[1].setOrigin(50, 25);

	iconBG[2].setSize(sf::Vector2f(30, 150));
	iconBG[2].setOrigin(15, 75);

	//overlay.setSize(sf::Vector2f(4480, 2560));
	//overlay.setFillColor(DARK_OVERLAY);

	if (!font.loadFromFile(FONT))
	{

	}
	text.setFont(font);
	text.setCharacterSize(18U);

	m_scoreText.setFont(font);
	m_scoreText.setCharacterSize(30U);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setOutlineColor(sf::Color::Black);
	m_scoreText.setString("000000");

	m_scoreIncreaseText.setFont(font);
	m_scoreIncreaseText.setCharacterSize(30U);
	m_scoreIncreaseText.setFillColor(sf::Color::Yellow);
	m_scoreIncreaseText.setOutlineColor(sf::Color::Black);
	m_scoreIncreaseText.setString("+Score");
	

}

void HUD::idleAnimation()
{

	idleTime++;
	if (idleTime > 12)//5 is the speed at how fast they animate
	{
		idleFrame++;
		if (idleFrame > 1) //frame 6 is the last frame the robot is running
		{
			idleFrame = 0; //0 is the first frame the robot is running
		}
		idleTime = 0;
	}
	int col = idleFrame % 2; //total cols is 5
	int row = 0; //row 0

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 32;
	rectSourceSprite.width = 32;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	playerSprite.setTextureRect(rectSourceSprite);
}

void HUD::followCamera(sf::Vector2f _pos)
{
	iconBG[0].setPosition(_pos.x -400, _pos.y - 175);
	iconBG[1].setPosition(iconBG[0].getPosition().x,iconBG[0].getPosition().y+75);

	iconBG[2].setPosition(iconBG[0].getPosition().x + 75, iconBG[0].getPosition().y + 25 );

	playerSprite.setPosition(iconBG[0].getPosition().x, iconBG[0].getPosition().y);
	gunSprite.setPosition(iconBG[0].getPosition().x-10, iconBG[1].getPosition().y);

	//healthBar.setPosition(iconBG[0].getPosition().x - 75, iconBG[0].getPosition().y - 75);
	healthRect.setPosition(iconBG[2].getPosition().x, iconBG[2].getPosition().y + 80);

	text.setString(std::to_string(ammoCount));
	text.setPosition(gunSprite.getPosition().x + 35, gunSprite.getPosition().y-10);

	m_scoreText.setPosition(iconBG[0].getPosition().x - 50, iconBG[0].getPosition().y-85);
	for (int i = 0; i < m_scoreIncreaseTexts.size(); i++)
	{
		if (!m_scoreIncreaseTexts[i].update(sf::Vector2f(iconBG[0].getPosition().x - 50, iconBG[0].getPosition().y - 85))) //returns a bool if they are on screen
		{
			m_scoreIncreaseTexts.erase(m_scoreIncreaseTexts.begin() + i);
		}
	}
	
}

void HUD::setGunHudTexture(sf::Texture& text)
{
	gunSprite.setTexture(text);
}

void HUD::setAmmoHudNumber(int t_ammoCount)
{
	ammoCount = t_ammoCount;
}

void HUD::adjustHealthBar(int t_health)
{
	float healthWidth = maxHealthBarRectSize.y / s_MAX_PLAYER_HEALTH; //health width is size of 1 singular health.
	healthWidth *= t_health;
	healthBarRectSize.y = healthWidth;

	if (healthBarRectSize.y >= 0)
	{
		healthBarRectSize.y = 0;
	}
}

void HUD::updateScore(float t_score)
{
	std::ostringstream scoreString;
	scoreString << std::setw(6) << std::setfill('0') << static_cast<int>(t_score);
	m_scoreText.setString(scoreString.str());

	score = t_score;
}

void HUD::updateIncrease(int t_scoreIncrease)
{
	std::string scoreString = "+" + std::to_string(t_scoreIncrease);
	ScoreIncreaseText scoreInreaseText;
	scoreInreaseText.setFont(font);
	scoreInreaseText.setCharacterSize(30U);
	scoreInreaseText.currentColor = (sf::Color::Yellow);
	scoreInreaseText.setFillColor(scoreInreaseText.currentColor);
	scoreInreaseText.setOutlineColor(sf::Color::Black);
	scoreInreaseText.setString(scoreString);
	
	m_scoreIncreaseTexts.push_back(scoreInreaseText);
}
