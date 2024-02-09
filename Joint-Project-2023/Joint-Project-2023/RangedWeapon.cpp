#include "RangedWeapon.h"
#include "Thor/Vectors.hpp"
#include "Globals.h"

RangedWeapon::RangedWeapon(sf::Texture& t_weaponTex, sf::Texture& t_projectileTex, RangedWeaponTypes t_type, int t_ammoCount) :
	m_weaponTexture(t_weaponTex),
	m_projectileTexture(t_projectileTex),
	m_rangedType(t_type),
	ammoCount(t_ammoCount)
{
	this->m_weaponType = WeaponType::Ranged;
	m_projectilePool = std::make_unique<ProjectilePool>();

	weaponSprite.setTexture(t_weaponTex);
	weaponSprite.setOrigin(100, 100);
	weaponSprite.setScale(0.25, 0.25);

	initSounds();

	switch (t_type)
	{
	case RangedWeaponTypes::Pistol:
		fireRate = sf::seconds(0.4);
		break;
	case RangedWeaponTypes::Shotgun:
		fireRate = sf::seconds(1);
		break;
	case RangedWeaponTypes::Rifle:
		fireRate = sf::seconds(0.2);
		break;
	default:
		break;
	}
}

RangedWeapon::~RangedWeapon()
{

}

void RangedWeapon::update(sf::Time& t_deltaTime, sf::Vector2f t_pos)
{
	m_projectilePool->update(t_deltaTime);
	updateWeaponSpritePos(t_pos);
}

void RangedWeapon::render(sf::RenderWindow& t_window)
{
	m_projectilePool->render(t_window);
	t_window.draw(weaponSprite);
}

void RangedWeapon::attack(sf::Vector2f t_position, sf::Vector2f t_velocity)
{
	if (ammoCount > 0)
	{
		if (timeSinceLastProjectileFired.getElapsedTime() > fireRate || firstPickUp == true) //delay how fast we fire
		{
			firstPickUp = false;
			switch (m_rangedType)
			{
			case RangedWeaponTypes::Pistol:
				pistolShot.play();
				m_projectilePool->createProjectile(m_projectileTexture, t_position, t_velocity);
				timeSinceLastProjectileFired.restart();
				ammoCount -= 1;
				break;
			case RangedWeaponTypes::Shotgun:
				shotgunShot.play();
				fireShotgun(t_position, t_velocity);
				ammoCount -= 1;
				break;
			case RangedWeaponTypes::Rifle:
				rifleShot[currentRifleShot].play(); //plays sound
				currentRifleShot++; //increases current shot so sounds can overlap
				if (currentRifleShot >= 10) //if shots are greater than max 
				{
					currentRifleShot = 0; //change to 0
				}
				m_projectilePool->createProjectile(m_projectileTexture, t_position, t_velocity);
				timeSinceLastProjectileFired.restart();
				ammoCount -= 1;
				break;
			default:
				break;
			}

		}
		
	}	
}

/// <summary>
/// rotates the weapon based on the angle of where the mopuse is
/// </summary>
/// <param name="_angle">Angle of mouse</param>
/// <param name="_leftorRight">if scale to left or right of player</param>
void RangedWeapon::rotateWeapon(float _angle, int _leftorRight)
{
	weaponSprite.setRotation(_angle);
	if (_leftorRight == 1)
	{
		weaponSprite.setScale(0.25, -0.25);
	}
	else
	{
		weaponSprite.setScale(0.25, 0.25);
	}
}

/// <summary>
/// fires bullets in a spread direction
/// </summary>
/// <param name="t_position">player Position</param>
/// <param name="t_velocity">direction towards mouse</param>
void RangedWeapon::fireShotgun(sf::Vector2f t_position, sf::Vector2f t_velocity)
{	
	int numberOfShotgunPellets = 3;
	float firingCone = 20; //degree cone of shotgun pellet spread
	float angleDecrement = firingCone / (numberOfShotgunPellets / 3.f);
	sf::Vector2f velocity;
	//rotate velocity to start of cone spread, then as we rotate clockwise , fire more pellets
	for (int i = 0; i < numberOfShotgunPellets; i++)
	{
		velocity = t_velocity; //reset velocity in direction of mouse
		thor::rotate(velocity, firingCone); //rotate velocity vector where bullet will head
		m_projectilePool->createProjectile(m_projectileTexture, t_position, velocity); //shoot in that direction
		firingCone -= angleDecrement; //reduce rotation so next bullet fires at an agle clockwise to that
	}
	
	timeSinceLastProjectileFired.restart();
}

/// <summary>
/// updates visable sprite of weapon on player
/// </summary>
/// <param name="t_position">position of player</param>
void RangedWeapon::updateWeaponSpritePos(sf::Vector2f t_position)
{
	weaponSprite.setPosition(sf::Vector2f(t_position.x, t_position.y + 5));
}

void RangedWeapon::initSounds()
{
	pistolBuffer.loadFromFile(PISTOL_AUDIO);
	pistolShot.setBuffer(pistolBuffer);

	rifleBuffer.loadFromFile(RIFLE_AUDIO);
	for (int i = 0; i < 10; i++)
	{
		rifleShot[i].setBuffer(rifleBuffer);
	}

	shotgunBuffer.loadFromFile(SHOTGUN_AUDIO);
	shotgunShot.setBuffer(shotgunBuffer);
}
