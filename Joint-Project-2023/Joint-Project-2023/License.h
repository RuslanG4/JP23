#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>
#include"Defines.h"
class License
{
private:
	sf::Text text;
	sf::Font font;

public:
	void render(sf::RenderWindow& window);
	void initialise(sf::Font& t_font);
	void update();

	sf::Clock m_clock;
	sf::Time licenseTime = sf::seconds(1.5);
};

