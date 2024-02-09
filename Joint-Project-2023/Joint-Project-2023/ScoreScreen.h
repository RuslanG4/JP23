#pragma once
#include <SFML/Graphics.hpp>
#include"Defines.h"
#include<iostream>
class ScoreScreen
{
public:
	void update(sf::Time& t_deltaTime, sf::RenderWindow& t_window);
	void render(sf::RenderWindow& t_window);
	void processInput(sf::Event& t_event);
	void initialise(sf::Font & font);
	void textBoundary();
	void increaseButtonOpacity();

	void setVariables(int _score, int _enemiesKilled);

	bool reset{ false };


	static const int numOfGameValues = 3;
private:
	sf::Text score; //text for score
	sf::Text enemiesKilled; ///text for enemies killed
	sf::Text gameRating; //text for game rating

	sf::Font myFont;

	sf::View scoreScreenView;//reset view for showing;

	sf::Sprite m_buttonSprite[2]; //sprites for buttons

	sf::Texture m_buttonTxtWhite; //white button texture

	sf::Texture m_buttonTxtRed; //red button texture;

	float buttonOpacity = 0;

	int scoreNum=1234;//gamescvore
	int enemiesKilledNum = 4;//enemies killed in game
	char grade = 'a'; //grade at the end of the game

	sf::Text GameValues[numOfGameValues]; //to store score, killed and grades
	void initGameValues();

	bool movedScore{ false };
	bool movedEnemiesKilled{ false };
	bool movedGrade{ false };

	void showEndgameAnimation();
	int tempScore = 0;
	int tempEnemiesKileed = 0;

	void setGrade();

	
};

