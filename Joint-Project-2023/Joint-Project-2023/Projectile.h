#pragma once
#include <SFML/Graphics.hpp>
class Projectile
{
public:
	Projectile();

	bool update(sf::Time& t_deltaTime);
	void render(sf::RenderWindow& t_window);
	void init(sf::Texture& t_textire, sf::Vector2f t_position, sf::Vector2f t_velocity);

	sf::FloatRect getBounds() { return m_projectileSprite.getGlobalBounds(); }
	void setPositionVector(sf::Vector2f t_position) { m_position = t_position; m_projectileSprite.setPosition(m_position); }
	void setVelocity(sf::Vector2f t_velocity) { m_velocity = t_velocity; }
	float getSpeed() { return m_speed; }
	bool isOnScreen() { return m_onScreen; }
	void setOnScreen(bool t_value) { m_onScreen = t_value; }
	sf::Vector2f getPosition() { return m_position; }
	void rotateBulletSprite(sf::Vector2f t_velocity);
	
private:
	sf::Sprite m_projectileSprite;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity{1.f,1.f};
	float m_speed{ 15 };
	bool m_onScreen{ false };
};

