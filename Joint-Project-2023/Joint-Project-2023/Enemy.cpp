#include "Enemy.h"
#include "Thor/Vectors.hpp"
#include "Thor/Math.hpp"
#include "MathUtility.h"

/// <summary>
/// anything to do with upodateing the enemy
/// </summary>
void Enemy::update(sf::Time& t_deltaTime)
{
	if (isAlive)
	{
		sf::Vector2f acceleration;

		switch (m_currentState)
		{
		case BehaviourState::None:

			break;
		case BehaviourState::PathFinding:
			m_position.x += m_velocity.x * speed * t_deltaTime.asSeconds();
			m_position.y += m_velocity.y * speed * t_deltaTime.asSeconds();

			break;
		case BehaviourState::Tracking:
			
			//velocity will always be tracking player here
			if (m_velocity.x != 0 || m_velocity.y != 0)
			{
				m_steering += thor::unitVector(m_velocity);
				m_steering += collisionAvoidance();
				m_steering = MathUtility::truncate(m_steering, MAX_FORCE);
				acceleration = m_steering;
				m_velocity = MathUtility::truncate(m_velocity + acceleration, speed);
				m_velocity = thor::unitVector(m_velocity);
			}
			

			m_position.x += m_velocity.x  * speed * t_deltaTime.asSeconds();
			m_position.y += m_velocity.y  *  speed * t_deltaTime.asSeconds();
			
			break;
		case BehaviourState::WithinAttackRange:

			break;
		default:
			break;
		}
		
		meleeDetectionRadius.setPosition(m_position);

		_attackRadius.setPosition(m_position);

		m_enemySprite.setPosition(m_position);

		enemyHB.setPosition(m_enemySprite.getPosition());

		m_playerWeapon->update(t_deltaTime, m_enemySprite.getPosition());

		animateCharacter();

		initHealthBar();

		if (m_playerWeapon->getWeaponType() == WeaponType::Melee)
		{
			if (attackAnimation)
			{
				animateAttack();
			}
		}
		if (isImmune)
		{
			immuneTime++;
			if (immuneTime > 30)
			{
				immuneTime = 0;
				isImmune = false;
			}
		}

	}
	if(!isAlive)
	{
		animateDeath();
	}
}

/// <summary>
/// loops through the projectiles and sees if its global bounds connects with the enemy sprite
/// </summary>
/// <param name="t_projectilePool"></param>
/// <returns>true if collision, false if not</returns>
bool Enemy::checkProjectileCollisions(std::shared_ptr<ProjectilePool>& t_projectilePool, RangedWeaponTypes t_type)
{
	std::array<Projectile, s_POOL_SIZE> &projectiles = t_projectilePool->getProjectiles(); //store projectiles in  an array
	bool collision = false;

	for (unsigned int i = 0; i < projectiles.size(); i++) //loop through size of array
	{
		if (projectiles[i].isOnScreen() == true)
		{
			if (enemyHB.getGlobalBounds().intersects(projectiles[i].getBounds()))  //checks collision between sprite rectangles
			{
				takeDamage(t_type);
				projectiles[i].setOnScreen(false);
				collision = true; //if yes
			}
		}			
	}

	return collision;

}

/// <summary>
/// gets melee attack area and checks its bounds with enemy bounds
/// </summary>
/// <param name="t_weaponAttackArea"></param>
/// <returns> true if both collide, false if not </returns>
bool Enemy::checkMeleeCollisions(sf::FloatRect t_weaponAttackArea)
{
	if (enemyHB.getGlobalBounds().intersects(t_weaponAttackArea)) //check global collision
	{
		return true; //if yes
	}
	return false; //else
}


void Enemy::setMeleeWeapon(sf::Texture& t_weaponTexture, MeleeWeaponType t_type)
{
	m_playerWeapon = std::make_unique<MeleeWeapon>(t_weaponTexture, t_type);
}


void Enemy::chase(sf::Vector2f _playerPos)
{
	m_prevPos = m_position - sf::Vector2f((2 * m_velocity.x), (2 * m_velocity.y));
	sf::Vector2f chaseDirection = _playerPos - m_position;
	thor::setLength(chaseDirection, float(2));
	m_velocity = chaseDirection;
	m_currentState = BehaviourState::Tracking;
	while (path.size() > 0) //clear path since we are tracking
	{
		path.pop();
	}
}

sf::Vector2f Enemy::collisionAvoidance()
{
	//get vector in direction and set it to the maximum see ahead so it doesent avoid obstacles prematurely
	headingVector = m_velocity;
	thor::setLength(headingVector, MAX_SEE_AHEAD);
	headingVector += m_position;
	sf::Vector2f avoidance(0, 0);

	findMostThreateningObstacle();

	if (thor::length(m_mostThreatening.getPosition() - headingVector) < m_mostThreatening.getRadius() * 1.55 &&
		thor::length(m_mostThreatening.getPosition() - headingVector) > m_mostThreatening.getRadius() * 0.55)
	{
		avoidance.x = headingVector.x - m_mostThreatening.getPosition().x;
		avoidance.y = headingVector.y - m_mostThreatening.getPosition().y;
		avoidance = thor::unitVector(avoidance);
		avoidance *= MAX_AVOID_FORCE;
	}
	else
	{
		avoidance *= 0.0f;
	}
	return avoidance;
}

void Enemy::findMostThreateningObstacle()
{	
	sf::Vector2f rectPos;
	sf::CircleShape mostThreatening;
	float avoidanceDistance = 20;

	for (int i = 0; i < m_collisionGrid.size(); i++)
	{
		rectPos = { m_collisionGrid[i].left,m_collisionGrid[i].top };
		if (thor::length(rectPos - m_position) < 400)
		{
			mostThreatening.setRadius(m_collisionGrid[i].width * 1.5f);
			mostThreatening.setOrigin(mostThreatening.getRadius(), mostThreatening.getRadius());
			mostThreatening.setPosition(rectPos);

			if (thor::length(mostThreatening.getPosition() - headingVector) < mostThreatening.getRadius())
			{
				m_mostThreatening = mostThreatening;
			}
		}
	}
}




void Enemy::setRangedWeapon(sf::Texture& t_weaponTexture, sf::Texture& t_projectileTexture, RangedWeaponTypes t_type, int t_ammoCount)
{
	m_playerWeapon = std::make_unique<RangedWeapon>(t_weaponTexture, t_projectileTexture, t_type, t_ammoCount);
}


void Enemy::attack(sf::Vector2f _playerPos)
{
	sf::Vector2f velocity =_playerPos - m_enemySprite.getPosition();
	m_playerWeapon->attack(m_enemySprite.getPosition(), velocity);
	attackAnimation = true;
}
/// <summary>
/// initialises variables such as loading sprite and positions
/// </summary>
void Enemy::init()
{
	initSounds();

	if (!texture.loadFromFile(ENEMY_SPRITE)) //load sprite sheet texture
	{
		std::cout << "error loading sprite sheet";
	}


	m_enemySprite.setTexture(texture); //sets texture
	m_enemySprite.setTextureRect(sf::IntRect(0, 350, 150, 150)); //bounds on the sprite sheet
	m_enemySprite.setOrigin(75, 75); //origin in middle of sprite
	m_enemySprite.setScale(2, 2); //scale up by 2

	//position = sf::Vector2f(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT); //sets random position on screen

	m_enemySprite.setPosition(m_position); //sets enemy on a random position in screen based on screen width and height, change after collisions

	_attackRadius.setSize(sf::Vector2f(300, 300));
	_attackRadius.setOrigin(150, 150);
	_attackRadius.setFillColor(sf::Color(255, 0, 0, 68));
	_attackRadius.setPosition(m_enemySprite.getPosition());

	enemyHB.setSize(sf::Vector2f(50, 75));
	enemyHB.setOrigin(25, 25);
	enemyHB.setFillColor(sf::Color::Transparent);
	enemyHB.setOutlineThickness(3);


	meleeDetectionRadius.setSize(sf::Vector2f(600, 600));
	meleeDetectionRadius.setOrigin(300, 300);
	meleeDetectionRadius.setFillColor(sf::Color(255, 0, 0, 68));
	meleeDetectionRadius.setPosition(m_enemySprite.getPosition());

	
}
/// <summary>
/// draws all things to do with enemy
/// </summary>
/// <param name="t_win">window that draws</param>
void Enemy::render(sf::RenderWindow& t_win)
{
	if (DEBUG_MODE)
	{
		t_win.draw(meleeDetectionRadius);
		t_win.draw(_attackRadius);
		t_win.draw(enemyHB);
	}
	
	
	
	t_win.draw(m_enemySprite); //draws enemy
	
	t_win.draw(_healthbar);
	m_playerWeapon->render(t_win);
}

void Enemy::updateWeaponRotation(sf::Vector2f& _playerPos)
{
	sf::Vector2f rotationVector = _playerPos - m_enemySprite.getPosition();
	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / PI;
	rotation = -rotation; //sfml window is upside down so negative rotation
	rotation += 90; //sprite facing rightways

	if (_playerPos.x < m_enemySprite.getPosition().x)
	{
		m_playerWeapon->rotateWeapon(rotation, 1);
	}
	else
	{
		m_playerWeapon->rotateWeapon(rotation, 2);
	}
}

void Enemy::animateCharacter()
{
	if(m_velocity.x > 0 || m_velocity.x<0 || m_velocity.y>0||m_velocity.y<0)
	{
		if (m_velocity.x > 0)
		{
			m_enemySprite.setScale(2, 2);
		}
		if (m_velocity.x < 0)
		{
			m_enemySprite.setScale(-2, 2);
		}
		animateWalk();
	}
	else
	{
		animateIdle();
	}
}

void Enemy::animateIdle()
{
	idleTime++;
	if (idleTime > 12)//5 is the speed at how fast they animate
	{
		idleFrame++;
		if (idleFrame > 3) //frame 6 is the last frame the robot is running
		{
			idleFrame = 0;

		}
		idleTime = 0;
	}
	int col = idleFrame % 4; //total cols is 5
	int row = 3; //row 0

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 150;
	rectSourceSprite.width = 150;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	m_enemySprite.setTextureRect(rectSourceSprite);
}

void Enemy::animateWalk()
{
	runTime++;
	if (runTime > 4)//5 is the speed at how fast they animate
	{
		runFrame++;
		if (runFrame > 7) //frame 6 is the last frame the robot is running
		{
			runFrame = 0; //0 is the first frame the robot is running
		}
		runTime = 0;
	}
	int col = runFrame % 8; //total cols is 5
	int row = 0; //row 0

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 150;
	rectSourceSprite.width = 150;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	m_enemySprite.setTextureRect(rectSourceSprite);
}

void Enemy::takeDamage(RangedWeaponTypes _type)
{
	switch (_type)
	{
	case RangedWeaponTypes::Pistol:
		health -= 15;
		break;
	case RangedWeaponTypes::Shotgun:
		health -= 17;
		break; 
	case RangedWeaponTypes::Rifle:
		health -= 20;
			break;
	}
	if (health <= 0)
	{
		isAlive = false;
	}
}

void Enemy::takeDamage(MeleeWeaponType _type)
{
	switch (_type)
	{
	case MeleeWeaponType::Fist:
		health -= 25;
		break;
	default:
		break;
	}
	if (health <= 0)
	{
		isAlive = false;
	}
}

void Enemy::initHealthBar()
{
	_healthbar.setFillColor(sf::Color::Red);
	_healthbar.setSize(sf::Vector2f(health, 10));
	_healthbar.setPosition(m_position.x-50, m_position.y - 55);
}

void Enemy::playHitSound()
{
	hitSound[currentHitSound].play();
	currentHitSound++;
	if (currentHitSound > 4)
	{
		currentHitSound = 0;
	}
}

void Enemy::animateDeath()
{
	deathTime++;
	if (deathTime > 6)//6 is the speed at how fast they animate
	{
		deathFrame++;
		if (deathFrame > 3) //frame 6 is the last frame the robot is running
		{
			deSpawn = true;
		}
		deathTime = 0;
	}
	int col = deathFrame % 4; //total cols is 4
	int row = 2; //row 2

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 150;
	rectSourceSprite.width = 150;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	m_enemySprite.setTextureRect(rectSourceSprite);
}

void Enemy::animateAttack()
{
	attackTime++;
	if (attackTime > 3)//2 is the speed at how fast they animate
	{
		attackFrame++;
		if (attackFrame > 7) //frame 6 is the last frame the robot is running
		{
			attackFrame = 0; //0 is the first frame the robot is running
			attackAnimation = false;
		}
		attackTime = 0;
	}
	int col = attackFrame % 8; //total cols is 5
	int row = 1; //row 0

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 150;
	rectSourceSprite.width = 150;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	m_enemySprite.setTextureRect(rectSourceSprite);
}

void Enemy::initSounds()
{
	hitSoundBuffer.loadFromFile(ENEMY_HIT_AUDIO);
	for (int i = 0; i < 5; i++)
	{
		hitSound[i].setBuffer(hitSoundBuffer);
	}
}

