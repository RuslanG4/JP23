#pragma once
#include "ProjectilePool.h"
#include <SFML/Graphics.hpp>
enum class WeaponType
{
	None,
	Ranged,
	Melee
};

enum class MeleeWeaponType
{
	None,
	Fist,
};

enum class RangedWeaponTypes
{
	None,
	Pistol,
	Shotgun,
	Rifle
};

class Weapon
{
public:
	virtual void attack(sf::Vector2f t_position, sf::Vector2f t_velocity) = 0;
	virtual void update(sf::Time& _deltaTime,sf::Vector2f t_pos) = 0;
	virtual void render(sf::RenderWindow& t_window) = 0;

	virtual void rotateWeapon(float _angle, int _leftorRight) = 0;

	virtual WeaponType getWeaponType() { return m_weaponType; }
	virtual RangedWeaponTypes getRangedWeaponType() = 0;
	virtual MeleeWeaponType getMeleeWeaponType() = 0;

	virtual std::shared_ptr<ProjectilePool>& getProjectilePool() = 0;
	virtual sf::FloatRect getMeleeWeaponAttackArea() = 0;
	virtual int getAmmoCount() = 0;
	virtual void receiveAmmo(unsigned int t_ammoNum) = 0;
	virtual bool attacking() = 0;
	virtual void setAttacking(bool t_value) = 0;

	
	WeaponType m_weaponType{ WeaponType::None };
private:
	
};




