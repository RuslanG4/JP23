#pragma once
#include <SFML/Graphics.hpp>
#include "ScoreScreen.h"
#include "Player.h"
#include "Enemy.h"
#include"BloodParticle.h"
#include"DustParticle.h"
#include"Defines.h"
#include"MapCollisions.h"
#include "Crosshair.h"
#include "WeaponPickup.h"
#include"Grid.h"
#include"HUD.h"
#include<vector>



class GamePlay
{
public:
	GamePlay();
	void update(sf::Time& t_deltaTime);   //updates entities in game
	void render(sf::RenderWindow& t_window); //draws sprites
	void processInput(sf::Event& t_event); //checks input
	void initialise();  //initialises entities
	void initMap(); //sets up positions of red squares map is draw with a 16x16 tileset

	int getScore() { return myHud.getScore(); };
	int getEnemiesKilled() { return m_enemiesKilled; };

private:
	ScoreScreen m_scoreScreen;
	Player m_player;
	sf::Texture m_spriteSheetTexture;
	sf::Texture m_map;
	sf::Sprite m_bg;

	//WEAPONS
	std::vector<WeaponPickup> m_weaponPickups;
	std::vector<AmmoPickup> m_ammoPickup;
	sf::Texture m_rifleTexture;
	sf::Texture m_bulletTexture;
	sf::Texture m_shotgunTexture;
	sf::Texture m_pistolTexture;
	sf::Texture m_swordTexture;
	sf::Texture m_emptyTexture;

	std::vector<Enemy*> m_enemy;  //An array of shared pointers of type Enemy, we can allocate new Enemy pointers in array.
	void initialiseEnemies();
	void pathFind(Enemy* t_enemy);
	void updateEnemies(Enemy* t_enemy);
	void checkEnemySpawnTimer();
	void enemyChaseIfHit();
	int enemySpawnCount = 10;
	sf::Time m_spawnRate{ sf::seconds(10) };
	sf::Clock m_timeSurvived;
	sf::Clock m_timeSinceLastEnemySpawned;


	void checkMeleeDetectionForEnemy();


	//HUD

	HUD myHud;
	sf::Texture m_healthBarTexture;
	float m_gameScore{0};
	int scoreIncreaseThreshold = 10;
	int scoreIncrement = 100;
	float m_scoreIncreaseMultiplier = 1;
	int m_enemiesKilled = 0;
	void increaseScore();

	//MOUSE POSITION
	Crosshair m_crosshair;
	void mouseUpdate();
	sf::Vector2f mousePosition; //saves mouse position

	//CAMERA VIEW
	sf::View camera;
	void initView();  //initialises camera
	void shakeCamera(); //shakes the camera
	void cameraFollow(); //follows player center
	sf::View hudView;

	//CHECK COLLISION
	void checkCollision(); //checks bullet collision
	void checkWallCollision(std::shared_ptr<ProjectilePool> t_projectilePool); //checks bullet to wall collision;
	void checkPickUpCollision();
	void checkPlayerBulletCollision();
	void checkEnemyBulletCollision();
	void checkAmmoPickupCollision();

	//WEAPON INTERACTION
	void playerPickupWeapon(WeaponPickup t_pickup);
	void playerPickupRangedWeapon(RangedWeaponTypes t_rangedType, int t_ammoCout);
	bool pickerUp{ false };


	bool shake{ false }; //bool for enabling shaking of screen

	//TETURES FOR PARTICLES
	sf::Texture texture;
	sf::Texture dustTexture;
	//SEPERATE PARTICLE SYSTEMS
	thor::ParticleSystem dustParticleSystem;
	thor::ParticleSystem bloodParticleSystem;
	//TIMER AND CLOCK FOR AUTOMATION, CURRENTLY NOT IN USE
	thor::CallbackTimer explosionTimer;
	sf::Clock frameClock;

	//INIT PARTICLE SYSTEMS
	void initBlood();

	//folow dust particles
	void dustParticles();


	void mapColl();
	
	

	Grid myGrid; //grid class for pathfinding etc
	std::vector<sf::FloatRect> collisionGrid;
	std::vector<sf::FloatRect> spawnGrid;

	int timer = 0;


	//ENEMY DROPS
	void dropWeapon();
	void enemyDropWeapon(RangedWeaponTypes t_types, int _enemyNum);
	void killEnemy();

	sf::Vector2f bounce{0,0};
	int bounceTimer=0;
	void bouncePickUps();

	float deathCircleRadius = 2;
	sf::CircleShape deathCircle;
	void playCircle();

	sf::Music backTrack;

	sf::Sprite overlay;
	sf::CircleShape playerLight;
	sf::RenderTexture darknessTexture;
	void setUpPlayerLight();
	void lightUpdate();

};

