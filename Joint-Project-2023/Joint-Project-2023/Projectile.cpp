#include "Projectile.h"
#include "Thor/Math.hpp"
#include "Thor/Vectors.hpp"
#include <iostream>

Projectile::Projectile()
{

}

bool Projectile::update(sf::Time& t_deltaTime)
{
	if (m_onScreen)
	{
		m_position += m_velocity;
		m_projectileSprite.setPosition(m_position);
	}
	
	return m_onScreen;
}

void Projectile::render(sf::RenderWindow& t_window)
{
	if (m_onScreen)
	{
		t_window.draw(m_projectileSprite);
	}
}

void Projectile::init(sf::Texture& t_texture, sf::Vector2f t_position, sf::Vector2f t_velocity)
{
	m_projectileSprite.setTexture(t_texture);
	sf::Vector2u textureSize = t_texture.getSize();
	m_projectileSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
	m_projectileSprite.setOrigin(100, 100);
	m_projectileSprite.setScale(0.1f, 0.1f);
	m_onScreen = true;

	m_projectileSprite.setPosition(t_position); //set bullet to player position when firing
	m_position = t_position;
	sf::Vector2f m_currentBulletVelocity = thor::unitVector(t_velocity) * m_speed; //set bullet in that direction
	m_velocity = m_currentBulletVelocity; //we give it unit vector because in bullet class it multiplies by speed
	//rotateBulletSprite(t_velocity);
}

void Projectile::rotateBulletSprite(sf::Vector2f t_velocity)
{
	float rotation = 0.0f;
	sf::Vector2f rotationVector;
	rotationVector.x = t_velocity.x;
	rotationVector.y = t_velocity.y;

	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / thor::Pi;
	rotation -= 90;

	m_projectileSprite.setRotation(-rotation);
}
