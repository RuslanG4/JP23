#include "Player.h"
#include "Globals.h"

Player::Player()
{
	initSounds();
}

void Player::update(sf::Time& t_deltaTime)
{
	if (m_health > 0)
	{
		animate();
		m_playerController.update(t_deltaTime.asSeconds());
		m_playerSprite.setPosition(m_playerController.getPosition());
		m_playerWeapon->update(t_deltaTime, m_playerController.getPosition());

		playerHB.setPosition(sf::Vector2f(m_playerController.getPosition().x, m_playerController.getPosition().y));
	}
	if(m_health<=0)
	{
		animateDeath();
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

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_playerSprite);
	m_playerWeapon->render(t_window);
	//t_window.draw(playerHB);


	sf::Vector2i pixelPos = sf::Mouse::getPosition(t_window);
	sf::Vector2f worldPos = t_window.mapPixelToCoords(pixelPos);
	m_mousePos = worldPos;
	
}

void Player::attack()
{
	//Send velocity in since weapon could be ranged or melee
	sf::Vector2f velocity = m_mousePos - m_playerController.getPosition();
   	m_playerWeapon->attack(m_playerController.getPosition(),velocity );
}

void Player::setRangedWeapon(sf::Texture& t_weaponTexture, sf::Texture& t_projectileTexture, RangedWeaponTypes t_type, int t_ammoCount)
{
	m_playerWeapon = std::make_unique<RangedWeapon>(t_weaponTexture, t_projectileTexture,t_type, t_ammoCount);
}

void Player::setMeleeWeapon(sf::Texture& t_weaponTexture, MeleeWeaponType t_type)
{
	m_playerWeapon = std::make_unique<MeleeWeapon>(t_weaponTexture, t_type);
}

void Player::takeRangedDamage(Enemy& _enemy)
{

	if (_enemy.getWeapon()->getRangedWeaponType() == RangedWeaponTypes::Pistol)
	{
		m_health -= 10;
		hitSound[currentHitSound].play();
		currentHitSound++;
		if (currentHitSound > 4)
		{
			currentHitSound = 0;
		}
	}
	if (_enemy.getWeapon()->getRangedWeaponType() == RangedWeaponTypes::Rifle)
	{
		m_health -= 15;
		hitSound[currentHitSound].play();
		currentHitSound++;
		if (currentHitSound > 4)
		{
			currentHitSound = 0;
		}
	}
	if (_enemy.getWeapon()->getRangedWeaponType() == RangedWeaponTypes::Shotgun)
	{
		m_health -= 10;
		hitSound[currentHitSound].play();
		currentHitSound++;
		if (currentHitSound > 4)
		{
			currentHitSound = 0;
		}
	}
	
}

void Player::takeMeleeDamage(Enemy& _enemy)
{
	if (_enemy.getWeapon()->getMeleeWeaponType() == MeleeWeaponType::Fist)
	{
		m_health -= 10;
		hitSound[currentHitSound].play();
		currentHitSound++;
		if (currentHitSound > 4)
		{
			currentHitSound = 0;
		}
	}
}

void Player::processMouseMove(sf::Event t_event)
{
	rotatePlayer(m_mousePos);
}

void Player::init(sf::Texture& t_texture)
{
	m_playerTexture = t_texture;
	//m_bulletTexture = t_bulletTexture;
	//m_playerController = t_playerController;
	m_playerSprite.setTexture(t_texture);
	m_playerSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	m_playerSprite.setOrigin(16, 16);
	m_playerSprite.setScale(1, 1);

	playerHB.setSize(sf::Vector2f(30, 30)); //size of hb
	playerHB.setOrigin(15, 15); //origin in center
	playerHB.setFillColor(sf::Color::Transparent); //see through
	playerHB.setOutlineThickness(3.f); //outline thickness


}

void Player::rotatePlayer(sf::Vector2f t_mousePos)
{
	
	sf::Vector2f rotationVector = t_mousePos - m_playerController.getPosition();
	rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / PI;
	rotation = -rotation; //sfml window is upside down so negative rotation
	rotation += 90; //sprite facing rightways
	//playerHB.setRotation(rotation);
}

/// <summary>
/// reads all projectiles that the player weapon has
/// </summary>
/// <returns>array of projectiles</returns>
std::shared_ptr<ProjectilePool>& Player::getProjectilePool()
{
	return m_playerWeapon->getProjectilePool();
}

sf::FloatRect Player::getMeleeWeaponAttack()
{
	return m_playerWeapon->getMeleeWeaponAttackArea();
}

WeaponType Player::getWeaponArchetype()
{
	return m_playerWeapon->getWeaponType();
}

RangedWeaponTypes Player::getRangedWeaponType()
{
	return m_playerWeapon->getRangedWeaponType();
}

MeleeWeaponType Player::getMeleeWeaponType()
{
	return m_playerWeapon->getMeleeWeaponType();
}

void Player::animate()
{
	switch (m_playerController.getDirection())
	{
	case 0:
		animateIdle();
		break;
	case 1:
		animateRun();
		break;
	default:
		break;
	}
	if (m_mousePos.x < m_playerSprite.getPosition().x)
	{
		m_playerSprite.setScale(-2, 2);
		m_playerWeapon->rotateWeapon(rotation, 1);
	}
	else
	{
		m_playerSprite.setScale(2, 2);
		m_playerWeapon->rotateWeapon(rotation, 2);
	}
}

void Player::animateIdle()
{
	
		idleTime++;
		if (idleTime > 12)//5 is the speed at how fast they animate
		{
			idleFrame++;
			if (idleFrame > 1) //frame 6 is the last frame the robot is running
			{
				idleFrame = 0; //0 is the first frame the robot is running
			}
			idleTime = 0;
		}
		int col = idleFrame % 2; //total cols is 5
		int row = 0; //row 0

		sf::IntRect rectSourceSprite;
		rectSourceSprite.height = 32;
		rectSourceSprite.width = 32;
		rectSourceSprite.left = col * rectSourceSprite.width;
		rectSourceSprite.top = row * rectSourceSprite.height;
		m_playerSprite.setTextureRect(rectSourceSprite);

	
}

void Player::animateRun()
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
	int row = 3; //row 0

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 32;
	rectSourceSprite.width = 32;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	m_playerSprite.setTextureRect(rectSourceSprite);

}

void Player::animateDeath()
{
	deathTime++;
	if (deathTime >10)//5 is the speed at how fast they animate
	{
		deathFrame++;
		if (deathFrame > 3) //frame 6 is the last frame the robot is running
		{
			deathOver = true;
		}
		deathTime = 0;
	}
	int col = deathFrame % 4; //total cols is 5
	int row = 6; //row 0

	sf::IntRect rectSourceSprite;
	rectSourceSprite.height = 32;
	rectSourceSprite.width = 32;
	rectSourceSprite.left = col * rectSourceSprite.width;
	rectSourceSprite.top = row * rectSourceSprite.height;
	m_playerSprite.setTextureRect(rectSourceSprite);
}

void Player::initSounds()
{
	hitSoundBuffer.loadFromFile(PLAYER_HIT_AUDIO);
	
	for (int i = 0; i < 5; i++)
	{
		hitSound[i].setBuffer(hitSoundBuffer);
	}
}





