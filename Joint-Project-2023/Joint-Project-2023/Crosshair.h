#pragma once
#include <SFML/Graphics.hpp>
class Crosshair
{
public:
	Crosshair();
	void update(sf::Vector2f t_mousePos);
	void render(sf::RenderWindow& t_window);

	//comment

private:
	static const int MAX_CROSSHAIR_RECTS = 4;
	//How far the crosshair rects can go out and in from mouse pos
	static const int MAX_OUTWARD_DISTANCE = 40;
	static const int MAX_INWARD_DISTANCE = 10;
	sf::RectangleShape m_rects[MAX_CROSSHAIR_RECTS];
	sf::Vector2f m_rectSize{ 15,5 };

	sf::Vector2f m_rectPositions[MAX_CROSSHAIR_RECTS];
	sf::Vector2f m_rectOffsetPositions[MAX_CROSSHAIR_RECTS]; //offset position which will be added to mouse pos
	float crossHairMoveSpeed = 0.05f;
	sf::Vector2f m_rectVelocities[MAX_CROSSHAIR_RECTS]
	{
		{crossHairMoveSpeed , 0},
		{ 0, -crossHairMoveSpeed},
		{-crossHairMoveSpeed , 0},
		{ 0, crossHairMoveSpeed}
	};//which way crosshair will travel
	
	
	
};

