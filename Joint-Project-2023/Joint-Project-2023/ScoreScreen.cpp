#include "ScoreScreen.h"
#include"Menus.h"

void ScoreScreen::update(sf::Time& t_deltaTime, sf::RenderWindow& t_window)
{
	textBoundary();

	sf::Vector2i mouseLocation;
	mouseLocation = sf::Mouse::getPosition(t_window);

	if (mouseLocation.y > 800 && mouseLocation.y < 800 + 103)
	{
		if (mouseLocation.x > 600 && mouseLocation.x < 600 + 300)
		{

			m_buttonSprite[1].setTexture(m_buttonTxtRed);

		
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{

				Menus::m_currentGameState = GameStates::MainMenu;
			}
		}
		else
		{
			m_buttonSprite[1].setTexture(m_buttonTxtWhite);
		}
		if (mouseLocation.x > m_buttonSprite[0].getPosition().x && mouseLocation.x < m_buttonSprite[0].getPosition().x + 300)
		{

			m_buttonSprite[0].setTexture(m_buttonTxtRed);


			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				reset = true;
				Menus::m_currentGameState = GameStates::GamePlay;
			}
		}
		else
		{
			m_buttonSprite[0].setTexture(m_buttonTxtWhite);
		}
	}
	else
	{
		m_buttonSprite[1].setTexture(m_buttonTxtWhite);
		m_buttonSprite[0].setTexture(m_buttonTxtWhite);
	}
	
	showEndgameAnimation();
	setGrade();
	
}

void ScoreScreen::render(sf::RenderWindow& t_window)
{
	t_window.clear(GRAY);
	t_window.setView(scoreScreenView);
	t_window.draw(score);
	t_window.draw(enemiesKilled);
	t_window.draw(gameRating);

	t_window.setMouseCursorVisible(true);

	for (int i = 0; i < 2; i++)
	{
		t_window.draw(m_buttonSprite[i]);
	}

	if (movedScore)
	{
		t_window.draw(GameValues[0]);
	}
	if (movedEnemiesKilled)
	{
		t_window.draw(GameValues[1]);
		if (movedGrade)
		{
			t_window.draw(GameValues[2]);
		}
	}

	
}

void ScoreScreen::processInput(sf::Event& t_event)
{
}

void ScoreScreen::initialise(sf::Font& font)
{
	myFont = font;

	score.setFont(font);
	score.setString("SCORE : ");
	score.setCharacterSize(224U);
	score.setFillColor(sf::Color(166, 61, 64));
	score.setOutlineColor(sf::Color::Black);
	score.setOutlineThickness(3);
	score.setPosition(-400, 50);

	enemiesKilled.setFont(font);
	enemiesKilled.setString("Enemies Killed : ");
	enemiesKilled.setCharacterSize(224U);
	enemiesKilled.setFillColor(sf::Color(166, 61, 64));
	enemiesKilled.setOutlineColor(sf::Color::Black);
	enemiesKilled.setOutlineThickness(3);
	enemiesKilled.setPosition(-1000, 250);


	gameRating.setFont(font);
	gameRating.setString("Overall : ");
	gameRating.setCharacterSize(224U);
	gameRating.setFillColor(sf::Color(166, 61, 64));
	gameRating.setOutlineColor(sf::Color::Black);
	gameRating.setOutlineThickness(3);
	gameRating.setPosition(-600, 450);


	if (!m_buttonTxtRed.loadFromFile(MENU_RED))
	{
		std::cout << "Can't load button texture";
	}
	if (!m_buttonTxtWhite.loadFromFile(MENU_WHITE))
	{
		std::cout << "Can't load button texture";
	}

	m_buttonSprite[0].setTexture(m_buttonTxtWhite);
	m_buttonSprite[0].setPosition(SCREEN_WIDTH - 900, 800);
	m_buttonSprite[0].setTextureRect(sf::IntRect(0, 618, 600, 206));

	m_buttonSprite[1].setTexture(m_buttonTxtWhite);
	m_buttonSprite[1].setPosition(600, 800);
	m_buttonSprite[1].setTextureRect(sf::IntRect(1212, 206, 600, 206));

	m_buttonSprite[0].setScale(0.5, 0.5);
	m_buttonSprite[1].setScale(0.5, 0.5);

	m_buttonSprite[0].setColor(sf::Color(255, 255, 255, buttonOpacity));
	m_buttonSprite[1].setColor(sf::Color(255, 255, 255, buttonOpacity));

	scoreScreenView.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	scoreScreenView.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	initGameValues();
}

void ScoreScreen::textBoundary()
{
	score.move(35, 0);
	if (score.getPosition().x > SCREEN_WIDTH / 2 - 404)
	{
		movedScore = true;
		score.setPosition(SCREEN_WIDTH / 2 - 404, score.getPosition().y);
		enemiesKilled.move(35, 0);
		if (enemiesKilled.getPosition().x > SCREEN_WIDTH / 2 - 890)
		{
			movedEnemiesKilled = true;
			enemiesKilled.setPosition(SCREEN_WIDTH / 2 - 890, enemiesKilled.getPosition().y);
			gameRating.move(35, 0);
			if (gameRating.getPosition().x > SCREEN_WIDTH / 2 - 475)
			{
				movedGrade = true;
				gameRating.setPosition(SCREEN_WIDTH / 2 - 475, gameRating.getPosition().y);
				increaseButtonOpacity();
			}
		}
	}
	
}

/// <summary>
/// allows the buttons to fade in for a better effect
/// </summary>
void ScoreScreen::increaseButtonOpacity()
{
	buttonOpacity++;
	if (buttonOpacity > 255)
	{
		buttonOpacity = 255;
	}
	for (int i = 0; i < 2; i++)
	{
		m_buttonSprite[i].setColor(sf::Color(255, 255, 255, buttonOpacity));
	}
}

void ScoreScreen::setVariables(int _score, int _enemiesKilled)
{
	scoreNum = _score;
	enemiesKilledNum = _enemiesKilled;
}



void ScoreScreen::initGameValues()
{
	for (int currentText = 0; currentText < numOfGameValues; currentText++)
	{
		GameValues[currentText].setFont(myFont);
		GameValues[currentText].setCharacterSize(224U);
		GameValues[currentText].setFillColor(sf::Color(166, 61, 64));
		GameValues[currentText].setOutlineColor(sf::Color::Black);
		GameValues[currentText].setOutlineThickness(3);
	}
	GameValues[0].setString("10000");
	GameValues[0].setPosition( 1100, 50);

	GameValues[1].setString("10000");
	GameValues[1].setPosition(1100, 250);

	GameValues[2].setPosition(1100, 450);
}

void ScoreScreen::showEndgameAnimation()
{
	if (movedScore)
	{
		tempScore+=50;
		if (tempScore < scoreNum)
		{
			GameValues[0].setString(std::to_string(tempScore));
		}
		else
		{
			GameValues[0].setString(std::to_string(scoreNum));
			GameValues[2].setString(grade);
		}
	}
	if (movedEnemiesKilled)
	{
		tempEnemiesKileed += 1;
		if (tempEnemiesKileed < enemiesKilledNum)
		{
			GameValues[1].setString(std::to_string(tempEnemiesKileed));
		}
		else
		{
			GameValues[1].setString(std::to_string(enemiesKilledNum));
		}
	}
}

void ScoreScreen::setGrade()
{
	if (scoreNum < 5000)
	{
		grade = 'D';
	}
	if (scoreNum > 5000 && scoreNum< 10000)
	{
		grade = 'C';
	}
	if (scoreNum > 10000 && scoreNum < 15000)
	{
		grade = 'B';
	}
	if (scoreNum > 15000 && scoreNum < 20000)
	{
		grade = 'A';
	}
	if (scoreNum > 25000)
	{
		grade = 'S';
	}

}
