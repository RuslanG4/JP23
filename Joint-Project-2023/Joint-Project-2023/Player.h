#pragma once
#include "Enemy.h"
#include "Weapon.h"
#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "PlayerController.h"
#include "Projectile.h"
#include <iostream>
#include <memory>
#include"HUD.h"
#include"SFML/Audio.hpp"


class Player
{
public:
	Player();
	virtual ~Player() {} //Virtual deconstructor since we are inheriting abstract interface i think

	void init(sf::Texture& t_texture);

	//Player
	void update(sf::Time &t_deltaTime);
	void render(sf::RenderWindow& t_window);
	void attack();

	void setRangedWeapon(sf::Texture& t_weaponTexture,sf::Texture& t_projectileTexture, RangedWeaponTypes t_type, int t_ammoCount);
	void setMeleeWeapon(sf::Texture& t_weaponTexture, MeleeWeaponType t_type);
	void takeRangedDamage(Enemy& _enemy);
	void takeMeleeDamage(Enemy& _enemy);
	void processMouseMove(sf::Event t_event);

	void rotatePlayer(sf::Vector2f t_mousePos);

	sf::Vector2f getPostion() { return m_playerController.getPosition(); };
	int getAmmoCount() { return m_playerWeapon->getAmmoCount(); }
	int getHealth() { return m_health; }
	bool isMoving() { return m_playerController.getMoving(); };

	//return projectiles for collision check in gameplay
	std::shared_ptr<ProjectilePool>& getProjectilePool();
	sf::FloatRect getMeleeWeaponAttack();

	WeaponType getWeaponArchetype();
	RangedWeaponTypes getRangedWeaponType();
	MeleeWeaponType getMeleeWeaponType();

	sf::RectangleShape getHB() { return playerHB; }; //gets player hitbox

	void stopVelocity()
	{
		m_playerController.setVelocityTo0();
	}

	void animate();
	void animateIdle();
	void animateRun();
	void animateDeath();


	std::shared_ptr<Weapon>& getWeapon() {
		return m_playerWeapon;
	};

	bool getDeathOver() { return deathOver; };

	bool isImmune{ false };
	

private:
	sf::Sprite m_playerSprite;
	sf::Texture m_playerTexture;
	PlayerController m_playerController;
	sf::RectangleShape playerHB; //hb for map collisions

	sf::Vector2f m_mousePos;
	
	std::shared_ptr<Weapon> m_playerWeapon;

	int idleTime = 0;
	int idleFrame = 0;

	int runTime = 0;
	int runFrame = 0;

	int deathTime = 0;
	int deathFrame = 0;
	bool deathOver{ false };

	float rotation = 0.0f;
	int m_health = s_MAX_PLAYER_HEALTH;

	int immuneTime = 0;

	int currentHitSound = 0;
	sf::Sound hitSound[5];
	sf::SoundBuffer hitSoundBuffer;
	void initSounds();

};

