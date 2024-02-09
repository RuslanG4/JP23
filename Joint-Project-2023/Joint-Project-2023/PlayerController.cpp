#include "PlayerController.h"
#include "PlayerController.h"
#include <iostream>

PlayerController::PlayerController(float t_x, float t_y)
{
	m_position.x = t_x;
	m_position.y = t_y;
}
PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::update(double t_deltaTime)
{
	isMoving = false;
	direction = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		direction = 1;
		m_prevPos.x = m_position.x;
		m_position.x -= m_velocty.x * t_deltaTime;
		isMoving = true;
		

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		direction = 1;
		m_prevPos.x = m_position.x;
		m_position.x += m_velocty.x * t_deltaTime;
		isMoving = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
	{
		direction = 1;
		m_prevPos.y = m_position.y;
		m_position.y -= m_velocty.y * t_deltaTime;
		isMoving = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
	{
		direction = 1;
		m_prevPos.y = m_position.y;
		m_position.y += m_velocty.y * t_deltaTime;
		isMoving = true;
	}
	
	//m_position = m_velocty;
}



