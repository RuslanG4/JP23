#include "MeleeWeapon.h"
#include <iostream>
#include "Thor/Math.hpp"
#include<SFML/Graphics.hpp>

MeleeWeapon::MeleeWeapon(sf::Texture& t_weaponTex, MeleeWeaponType t_type)
{
	this->m_weaponType = WeaponType::Melee;
	m_meleeWeaponTexture = t_weaponTex;
	m_meleeType = t_type;

	weaponSprite.setTexture(t_weaponTex);
	weaponSprite.setOrigin(46, 78);
	weaponSprite.setScale(0.25, 0.25);

	attackRectangle.setFillColor(sf::Color::Red);


	//Change diffrrent melee ranges
	switch (m_meleeType)
	{
	case MeleeWeaponType::Fist:

		m_meleeAreaSize = { 50,30 };

		break;
	default:
		break;
	}
	attackRectangle.setSize(m_meleeAreaSize);
	attackRectangle.setOrigin(0, m_meleeAreaSize.y / 2);


}

MeleeWeapon::~MeleeWeapon()
{
}

void MeleeWeapon::update(sf::Time& _deltaTime, sf::Vector2f t_pos)
{
	attackRectangle.setPosition(t_pos);
	weaponSprite.setPosition(sf::Vector2f(t_pos.x, t_pos.y));

}

void MeleeWeapon::render(sf::RenderWindow& t_window)
{
	//t_window.draw(attackRectangle);
	t_window.draw(weaponSprite);
}

void MeleeWeapon::attack(sf::Vector2f t_position, sf::Vector2f t_velocity)
{
	attacked = true;
}

bool MeleeWeapon::attacking()
{
	return attacked;
}

void MeleeWeapon::rotateWeapon(float _angle, int _leftorRight)
{
	swingRotation = _angle;
	if (!attacked)
	{
		weaponSprite.setRotation(_angle);
		attackRectangle.setRotation(_angle);
	}
	if (_leftorRight == 1)
	{
		weaponSprite.setScale(0.25, -0.25);
		if (attacked)
		{
			weaponSprite.setRotation(_angle -45);
			attackRectangle.setRotation(_angle -45);
		}
	}
	else
	{
		weaponSprite.setScale(0.25, 0.25);
		if (attacked)
		{
			weaponSprite.setRotation(_angle+45);
			attackRectangle.setRotation(_angle+45);
		}
	}
	
}

void MeleeWeapon::setPos(sf::Vector2f _pos)
{
	//m_meleeAttackArea.setPosition(_pos);
}
