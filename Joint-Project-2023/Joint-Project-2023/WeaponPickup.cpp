#include "WeaponPickup.h"
#include "Globals.h"

//For meleeTypes
WeaponPickup::WeaponPickup(sf::Texture& t_texture, sf::Vector2f t_pos, MeleeWeaponType t_meleeType)
{
	this->setTexture(t_texture);
	this->setPosition(t_pos);
	m_weaponArchetype = WeaponType::Melee;
	m_meleeType = t_meleeType;
}
//For rangedTypes
WeaponPickup::WeaponPickup(sf::Texture& t_texture, sf::Vector2f t_pos, RangedWeaponTypes t_rangedType, int t_ammoCount)
{
	this->setTexture(t_texture);
	this->setPosition(t_pos);
	switch (t_rangedType)
	{
	case RangedWeaponTypes::None:

		break;
	case RangedWeaponTypes::Pistol:
		this->setScale(0.25, 0.25);
		break;
	case RangedWeaponTypes::Shotgun:
		this->setScale(0.25, 0.25);
		break;
	case RangedWeaponTypes::Rifle:
		this->setScale(0.25, 0.25);
		break;
	default:
		break;
	}
	ammoCount = t_ammoCount;
	m_weaponArchetype = WeaponType::Ranged;
	m_rangedType = t_rangedType;
}

//no render since its a sprite

void WeaponPickup::update()
{
}
