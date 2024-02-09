#include "Crosshair.h"
#include <Thor/Vectors.hpp>
#include <iostream>

Crosshair::Crosshair()
{
	for (int i = 0; i < MAX_CROSSHAIR_RECTS; i++)
	{
		m_rects[i].setFillColor(sf::Color::White);
		m_rects[i].setSize(m_rectSize);
		m_rects[i].setOrigin(m_rectSize.x / 2, m_rectSize.y / 2);
		m_rects[i].setRotation(i * 90); //in sfml plane, 0 degrees is 90. so last rectangle is roated 360 even when 3 * 90 = 270
		//setup positions between the max and minimum distance from mouse
		m_rectOffsetPositions[i] = {m_rectVelocities[i].x* (MAX_OUTWARD_DISTANCE * 5), m_rectVelocities[i].y* (MAX_OUTWARD_DISTANCE * 5)};
	}
}

void Crosshair::update( sf::Vector2f t_mousePos)
{
	float distanceFromMouse;
	for (int i = 0; i < MAX_CROSSHAIR_RECTS; i++)
	{
		//rects originally are from mouse and then we add offset which is increase or decreased with velocity
		m_rectPositions[i] = m_rectOffsetPositions[i] + t_mousePos; 

		//if rectangle is too far out or in from mouse position
		distanceFromMouse = thor::length(m_rectPositions[i] - t_mousePos);
		if (distanceFromMouse > MAX_OUTWARD_DISTANCE || distanceFromMouse < MAX_INWARD_DISTANCE) 
		{
			m_rectVelocities[i] = -m_rectVelocities[i]; //reverse the direction it goes in
		}
		m_rectOffsetPositions[i] += m_rectVelocities[i]; //add velocity to the offset

		m_rects[i].setPosition(m_rectPositions[i]);
	}
}

void Crosshair::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < MAX_CROSSHAIR_RECTS; i++)
	{
		t_window.draw(m_rects[i]);
	}
}
