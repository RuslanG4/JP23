#pragma once
#include "Weapon.h"
#include "ProjectilePool.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include"defines.h"
#include <memory>


class RangedWeapon : public Weapon
{
public:
	RangedWeapon(sf::Texture& t_weaponTex,sf::Texture& t_projectileTex, RangedWeaponTypes t_type, int t_ammoCount);
	virtual ~RangedWeapon();
	void update(sf::Time& t_deltaTime, sf::Vector2f t_pos)override;
	virtual void render(sf::RenderWindow& t_window) override;
	void attack(sf::Vector2f t_position, sf::Vector2f t_velocity)override;
	std::shared_ptr<ProjectilePool>& getProjectilePool()override { return m_projectilePool; }
	sf::FloatRect getMeleeWeaponAttackArea() override { return sf::FloatRect(0, 0, 0, 0); }
	RangedWeaponTypes getRangedWeaponType() override { return m_rangedType; }
	MeleeWeaponType getMeleeWeaponType() override { return MeleeWeaponType::None; }
	int getAmmoCount()override { return ammoCount; }
	void receiveAmmo(unsigned int t_ammoNum)override { ammoCount += t_ammoNum; }
	bool attacking()override { return false; }
	void setAttacking(bool t_value) override { }

	void rotateWeapon(float _angle, int _leftorRight);

private:
	void fireShotgun(sf::Vector2f t_position, sf::Vector2f t_velocity);

	void updateWeaponSpritePos(sf::Vector2f t_position);

	std::shared_ptr<ProjectilePool> m_projectilePool;
	RangedWeaponTypes m_rangedType{ RangedWeaponTypes::None };
	sf::Texture& m_projectileTexture;
	sf::Texture& m_weaponTexture;

	sf::Sprite weaponSprite;

	int ammoCount{ 5 };
	bool firstPickUp{ true }; //this is so that when this weapon is just picked up, there is no delay in firing since we allocate a new weapon
	sf::Clock timeSinceLastProjectileFired; //this clock and time will delay how many bullets wee can shoot
	sf::Time fireRate;

	void initSounds();

	int currentRifleShot = 0;

	sf::Sound pistolShot;
	sf::SoundBuffer pistolBuffer;
	sf::Sound rifleShot[10];
	sf::SoundBuffer rifleBuffer;
	sf::Sound shotgunShot;
	sf::SoundBuffer shotgunBuffer;
};



