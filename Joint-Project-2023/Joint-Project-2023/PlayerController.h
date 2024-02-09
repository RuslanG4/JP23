#pragma once
#include<SFML/Graphics.hpp>

class PlayerController
{
public:
	PlayerController(float t_x, float t_y);
	PlayerController();
	~PlayerController();

	void update(double t_deltaTime);

	void setPosition(sf::Vector2f t_pos) { m_position = t_pos; }

	sf::Vector2f getPosition() { return sf::Vector2f(m_position.x , m_position.y ); }

	bool getMoving() { return isMoving; };

	void setVelocityTo0()
	{
		m_position = m_prevPos;
	}

	int getDirection() { return direction; };

private:
	sf::Vector2f m_position{ 1500.f,1000.f };
	sf::Vector2f m_prevPos{ 400,400 };
	sf::Vector2f m_velocty{ 300,300 };
	float speed{ 300 };
	float rotation{ 0 };
	bool isMoving{ false };
	int direction = 0;
	
};
