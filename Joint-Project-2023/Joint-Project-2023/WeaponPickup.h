#pragma once
#include <SFML/Graphics.hpp>
#include "Weapon.h"

class WeaponPickup : public sf::Sprite
{
public:
	WeaponPickup(sf::Texture& t_texture,sf::Vector2f t_pos, MeleeWeaponType t_meleeType);
	WeaponPickup(sf::Texture& t_texture,sf::Vector2f t_pos, RangedWeaponTypes t_rangedType,int t_ammoCount);

	WeaponType getWeaponArchetype() { return m_weaponArchetype; }
	MeleeWeaponType getMeleeType() { return m_meleeType; }
	RangedWeaponTypes getRangedType() { return m_rangedType; }
	int getAmmoCount() { return ammoCount; }

	void update();

private:
	WeaponType m_weaponArchetype = WeaponType::None;
	RangedWeaponTypes m_rangedType = RangedWeaponTypes::None;
	MeleeWeaponType m_meleeType = MeleeWeaponType::None;
	int ammoCount = 0;
};

class AmmoPickup : public sf::Sprite
{
public:
	AmmoPickup(sf::Texture& t_texture, sf::Vector2f t_pos)
	{
		this->setTexture(t_texture);
		this->setPosition(t_pos);
		this->setScale(0.1f, 0.1f);
	}
};

