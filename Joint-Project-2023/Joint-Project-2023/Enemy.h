#pragma once
#include "ProjectilePool.h"
#include"Defines.h"
#include<iostream>
#include"Weapon.h"
#include "MeleeWeapon.h"
#include"RangedWeapon.h"
#include <queue>
#include<Thor/Vectors.hpp>
#include<SFML/Audio.hpp>

enum class BehaviourState
{
	None,
	PathFinding,
	Tracking,
	WithinAttackRange
};

class Enemy
{
public:
	Enemy(std::vector<sf::FloatRect>&  t_collisionGrid) : m_collisionGrid(t_collisionGrid)
	{
		init();
	};

	void update(sf::Time& t_deltaTime);
	void init();
	void render(sf::RenderWindow& t_win);

	bool checkProjectileCollisions(std::shared_ptr<ProjectilePool>& t_projectilePool,RangedWeaponTypes t_type);
	bool checkMeleeCollisions(sf::FloatRect t_weaponAttackArea);

	void setVelocityVector(sf::Vector2f t_velocity) { m_velocity = t_velocity; }
	void setPath(std::queue<int>& t_path) { path = t_path; m_currentState = BehaviourState::PathFinding; }
	void setPosition(sf::Vector2f t_position) { m_position = t_position; }

	std::queue<int>& getPath() { return path; }
	sf::Vector2f getPosition() { return m_position; };

	//MELEE WEAPON
	void setMeleeWeapon(sf::Texture& t_weaponTexture, MeleeWeaponType t_type);

	std::shared_ptr<Weapon>& getWeapon() { return m_playerWeapon; }


	sf::FloatRect Enemy::getMeleeWeaponAttack()
	{
		return m_playerWeapon->getMeleeWeaponAttackArea();
	}

	sf::RectangleShape getMeleeDetection() { return meleeDetectionRadius; };

	void beginChase() {
		startChase = true;
	};

	void chase(sf::Vector2f _playerPos);
	void setBehaviourState(BehaviourState t_state) { m_currentState = t_state; }
	BehaviourState getBehaviourState() { return m_currentState; }

	static constexpr float MAX_SEE_AHEAD = 25.f;
	static float constexpr MAX_AVOID_FORCE{ 5.0f }; //maximum avoidance turn rate
	static float constexpr MAX_FORCE{ 30.0f };

	sf::Vector2f collisionAvoidance();
	void findMostThreateningObstacle();



	//RANGED WEAPON
	void attack(sf::Vector2f _playerPos);

	sf::RectangleShape getAttackRadius() { return _attackRadius; };
	sf::RectangleShape getRangedAttackRadius() { return meleeDetectionRadius; };

	void setRangedWeapon(sf::Texture& t_weaponTexture, sf::Texture& t_projectileTexture, RangedWeaponTypes t_type, int t_ammoCount);
	

	void updateWeaponRotation(sf::Vector2f& _playerPos);

	void animateCharacter();
	void animateIdle();
	void animateWalk();

	sf::RectangleShape getCollisionHB() { return enemyHB; };

	void prevPos() {
		m_position = m_prevPos;
	}

	void clearPath()
	{
		while (path.size() > 0)
		{
			path.pop();
		}
	}

	void takeDamage(RangedWeaponTypes _type);
	void takeDamage(MeleeWeaponType _type);

	void initHealthBar();

	bool checkDeSpawn() { return deSpawn; };

	bool startChase{ false };
	int getAttackAnimationFrame() { return attackFrame; };

	bool isImmune{ false };

	void playHitSound();

private:
	sf::Sprite m_enemySprite;
	sf::Vector2f m_position{ 400,400 };
	sf::Vector2f m_steering;
	sf::Vector2f headingVector;
	sf::Vector2f m_prevPos;
	sf::Vector2f m_velocity{ 0,0 };
	float speed = 100;
	sf::Texture texture;
	std::queue<int> path;
	std::shared_ptr<Weapon> m_playerWeapon;
	sf::RectangleShape _attackRadius;
	sf::RectangleShape meleeDetectionRadius;
	sf::RectangleShape enemyHB;
	float rotation= 0.0f;

	int idleTime = 0;
	int idleFrame = 0;

	int runTime = 0;
	int runFrame = 0;


	int deathTime = 0;
	int deathFrame = 0;
	void animateDeath();

	int attackTime = 0;
	int attackFrame = 0;
	void animateAttack();

	bool attackAnimation{ false };

	bool moving;

	int health{100};
	bool isAlive{ true };
	bool deSpawn{ false };

	int immuneTime = 0;

	BehaviourState m_currentState = BehaviourState::None;
	std::vector <sf::FloatRect>& m_collisionGrid;
	sf::CircleShape m_mostThreatening;

	sf::RectangleShape _healthbar;

	int currentHitSound = 0;
	sf::Sound hitSound[5];
	sf::SoundBuffer hitSoundBuffer;
	void initSounds();

};

