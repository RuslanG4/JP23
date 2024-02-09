#include "GamePlay.h"
#include <cmath>
#include"Menus.h"

GamePlay::GamePlay()
{
	initMap();
	initialise();
	setUpPlayerLight();
}

/// <summary>
/// updates every variable inside of gameplay
/// </summary>
/// <param name="t_deltaTime">time past</param>
void GamePlay::update(sf::Time& t_deltaTime)
{
	if (!m_player.getDeathOver())
	{

		mapColl();
		m_player.update(t_deltaTime);
		for (int i = 0; i < m_enemy.size(); i++)
		{
			m_enemy[i]->update(t_deltaTime);
			updateEnemies(m_enemy[i]);
		}

		bouncePickUps();

		m_gameScore += 1 * t_deltaTime.asSeconds();
		std::cout << "\n" << m_gameScore;


		dustParticleSystem.update(t_deltaTime);
		bloodParticleSystem.update(t_deltaTime);
		explosionTimer.update();

		dustParticles();

		cameraFollow();
		shakeCamera();

		checkCollision();

		mouseUpdate();

		myHud.update(t_deltaTime);
		myHud.updateScore(m_gameScore);


		checkMeleeDetectionForEnemy();

		myHud.adjustHealthBar(m_player.getHealth());

		dropWeapon();

		enemyChaseIfHit();

		//automatically remove weapon with no ammo
		if (m_player.getWeaponArchetype() == WeaponType::Ranged)
		{
			if (m_player.getAmmoCount() <= 0)
			{
				m_player.setMeleeWeapon(m_swordTexture, MeleeWeaponType::Fist);
			}
		}
		

		killEnemy();
		checkEnemySpawnTimer();

		lightUpdate();
	}

	if (m_player.getDeathOver())
	{
		playCircle();
	}
}
/// <summary>
/// draws everythign in the gameplay screen
/// </summary>
/// <param name="t_window"></param>
void GamePlay::render(sf::RenderWindow& t_window)
{
	t_window.setView(camera);
	t_window.draw(m_bg);
	t_window.draw(dustParticleSystem);
	t_window.draw(bloodParticleSystem);
	for (int i = 0; i < m_enemy.size(); i++)
	{
		m_enemy[i]->render(t_window);
	}
	

	/*for (int i = 0; i < myGrid.returnPathShapes().size(); i++)
	{
		t_window.draw(myGrid.returnPathShapes()[i]);
	}*/

	for (int i = 0; i < m_weaponPickups.size(); i++)
	{
		t_window.draw(m_weaponPickups[i]);
	}
	for (int i = 0; i < m_ammoPickup.size(); i++)
	{
		t_window.draw(m_ammoPickup[i]);
	}

	t_window.setMouseCursorVisible(false); //so mouse cursor not in the way of crosshair

	sf::Vector2i pixelPos = sf::Mouse::getPosition(t_window);
	sf::Vector2f worldPos = t_window.mapPixelToCoords(pixelPos);
	mousePosition = worldPos;

	t_window.draw(overlay);
	t_window.draw(playerLight);
	m_player.render(t_window);
	

	m_crosshair.update(mousePosition);
	m_crosshair.render(t_window);
	
	t_window.setView(hudView);
	myHud.render(t_window);

	t_window.draw(deathCircle);

}

void GamePlay::processInput(sf::Event& t_event)
{
	if (sf::Event::MouseButtonReleased == t_event.type)
	{
		if (sf::Mouse::Right == t_event.mouseButton.button  && !pickerUp)
		{
			checkPickUpCollision();

		}
		
	}
	if (sf::Event::MouseButtonPressed == t_event.type)
	{
		if (sf::Mouse::Right == t_event.mouseButton.button)
		{
			pickerUp = false;
		}
	}

	m_player.processMouseMove(t_event);
}

void GamePlay::initialise()
{
	if (!m_spriteSheetTexture.loadFromFile(CHAR_SPRITE)) //loads sprite sheet
	{
		std::cout << "error loading player texture";
	}
	if (!m_bulletTexture.loadFromFile(BULLET_SPRITE))
	{
		std::cout << "error loading player texture";
	}
	if (!m_rifleTexture.loadFromFile(RIFLE_SPRITE))
	{
		std::cout << "error loading player texture";
	}
	if (!m_shotgunTexture.loadFromFile(SHOTGUN_SPRITE))
	{
		std::cout << "error loading shotgun texture";
	}
	if (!m_pistolTexture.loadFromFile(PISTOL_SPRITE))
	{
		std::cout << "error loading shotgun texture";
	}
	if (!m_swordTexture.loadFromFile(SWORD_SPRITE))
	{
		std::cout << "error loading shotgun texture";
	}
	if (!m_emptyTexture.loadFromFile(EMPTY_SPRITE))
	{
		std::cout << "error loading shotgun texture";
	}
	if (!m_healthBarTexture.loadFromFile(HEALTH_BAR_SPRITE))
	{
		std::cout << "error loading healthBar texture";
	}
	if (!m_map.loadFromFile(MAP_SPRITE))
	{
		std::cout << "error loading map";
	}
	m_bg.setTexture(m_map);
	m_bg.setScale(2.f, 2.f);


	m_player.init(m_spriteSheetTexture); //gives player sprite sheet texture
	m_player.setRangedWeapon(m_rifleTexture, m_bulletTexture,RangedWeaponTypes::Rifle,MAX_RIFLE_BULLETS);  //gives player weapon type and bulle texture#


	//m_player.setMeleeWeapon(m_swordTexture, MeleeWeaponType::Fist);  //sets player weapon as fist

	if (!backTrack.openFromFile(BG_AUDIO))
	{
		//
	}
	backTrack.play();
	backTrack.setLoop(true);

	
	initBlood(); //initialise particles
	initView();  // initialise view and camera
	initialiseEnemies();

	//m_player.addEnemy(m_enemy); //gives player the enemy that was created in order to detect collision

	myHud.initialise(m_spriteSheetTexture,m_healthBarTexture);
	myHud.setGunHudTexture(m_rifleTexture);
	myHud.setAmmoHudNumber(MAX_RIFLE_BULLETS);

	deathCircle.setFillColor(GRAY);

}

/// <summary>
/// initialises enemy positions and weapons
/// </summary>
void GamePlay::initialiseEnemies()
{
	m_enemy.reserve(10);
	for (int i = 0; i < s_MAX_ENEMIES; i++)
	{
		int size = spawnGrid.size();
		int rando = rand() % size;
		m_enemy.push_back(new Enemy(collisionGrid)); //creates new enemy for the pointer array
		m_enemy[i]->setPosition(sf::Vector2f(spawnGrid.at(rando).left,spawnGrid.at(rando).top));

		rando = rand() % 100 + 1;
		if (rando < 50 && rando > 30)
		{
			int gunChoice = rand() % 3 + 1;
			switch (gunChoice)
			{
			case 1:
				m_enemy[i]->setRangedWeapon(m_pistolTexture, m_bulletTexture, RangedWeaponTypes::Pistol, 60);
				break;
			case 2:
				m_enemy[i]->setRangedWeapon(m_rifleTexture, m_bulletTexture, RangedWeaponTypes::Rifle, 80);
				break;
			case 3:
				m_enemy[i]->setRangedWeapon(m_shotgunTexture, m_bulletTexture, RangedWeaponTypes::Shotgun, 30);
				break;
			default:
				break;
			}
		}
		else
		{
			m_enemy[i]->setMeleeWeapon(m_emptyTexture, MeleeWeaponType::Fist);
		}
		pathFind(m_enemy[i]);
	}

	m_enemy[3]->setRangedWeapon(m_rifleTexture, m_bulletTexture, RangedWeaponTypes::Rifle, 80);
	m_enemy[3]->setPosition(sf::Vector2f(1500,1500));
}

/// <summary>
/// Sets the path for an enemy to follow to player using nodes
/// </summary>
/// <param name="m_enemy">Enemy</param>
void GamePlay::pathFind(Enemy* m_enemy)
{
	if (m_enemy->getBehaviourState() != BehaviourState::Tracking)
	{
		if (m_enemy->getPath().size() == 0) //if enemy is already following a path
		{
			sf::Vector2f enemyPos = m_enemy->getPosition();
			sf::Vector2f playerPos = m_player.getPostion();

			//find what nodes enemy and player are at
			int enemyNodeIndex = myGrid.nodeFromWorldPoint(enemyPos);
			int playerNodeIndex = myGrid.nodeFromWorldPoint(playerPos);

			Node enemyNode = myGrid.getNode(enemyNodeIndex);
			Node playerNode = myGrid.getNode(playerNodeIndex);

			std::queue<int> path = myGrid.findPath(enemyNode, playerNode);

			m_enemy->setPath(path);
		}
	}
	
}

void GamePlay::updateEnemies(Enemy* t_enemy)
{
	sf::Vector2f enemyPos = t_enemy->getPosition();
	std::queue<int>& path = t_enemy->getPath();
	if (path.size() > 0)
	{
		Node chaseNode = myGrid.getNode(path.front()); //get the node at the top of the queue
		sf::Vector2f nodePos{ static_cast<float>(chaseNode.gridX),static_cast<float>(chaseNode.gridY) };

		sf::Vector2f direction = nodePos - enemyPos;
		if (thor::length(direction) < 50)
		{
			path.pop();
			if (path.size() > 0)
			{
				chaseNode = myGrid.getNode(path.front()); //get the node at the top of the queue
				//Node x position isnt correct here
				nodePos = { static_cast<float>(chaseNode.gridX),static_cast<float>(chaseNode.gridY) };
				direction = nodePos - enemyPos;
			}		
		}
		sf::Vector2f unitDirection;
		if (direction.x != 0 || direction.y != 0)
		{
			unitDirection = thor::unitVector(direction);
			t_enemy->setVelocityVector(unitDirection);
		}
	
	}
	else
	{
		t_enemy->setVelocityVector({0,0});
	}
}

void GamePlay::checkEnemySpawnTimer()
{
	if (m_timeSinceLastEnemySpawned.getElapsedTime() > m_spawnRate)
	{
		m_timeSinceLastEnemySpawned.restart();

		if (m_enemy.size() < s_MAX_ENEMIES)
		{
			int enemySpawnCount = s_MAX_ENEMIES - m_enemy.size();
			for (int i = 0; i < enemySpawnCount; i++)
			{
				int size = spawnGrid.size();
				int rando = rand() % size;
				m_enemy.push_back(new Enemy(collisionGrid)); //creates new enemy for the pointer array
				m_enemy.back()->setPosition(sf::Vector2f(spawnGrid.at(rando).left, spawnGrid.at(rando).top));

				rando = rand() % 100 + 1;
				if (rando < 50 && rando > 30)
				{
					int gunChoice = rand() % 3 + 1;
					switch (gunChoice)
					{
					case 1:
						m_enemy.back()->setRangedWeapon(m_pistolTexture, m_bulletTexture, RangedWeaponTypes::Pistol, 60);
						break;
					case 2:
						m_enemy.back()->setRangedWeapon(m_rifleTexture, m_bulletTexture, RangedWeaponTypes::Rifle, 80);
						break;
					case 3:
						m_enemy.back()->setRangedWeapon(m_shotgunTexture, m_bulletTexture, RangedWeaponTypes::Shotgun, 30);
						break;
					default:
						break;
					}
				}
				else
				{
					m_enemy.back()->setMeleeWeapon(m_emptyTexture, MeleeWeaponType::Fist);
				}
				pathFind(m_enemy.back());
			}
		}
		
	}
}

void GamePlay::enemyChaseIfHit()
{
	for (int enemy = 0; enemy < m_enemy.size(); enemy++)
	{
		if (m_enemy[enemy]->startChase)
		{
			m_enemy[enemy]->chase(m_player.getPostion());
		}
	}
}

void GamePlay::checkMeleeDetectionForEnemy()
{
	for (int i = 0; i < m_enemy.size(); i++)
	{
		if (m_enemy.at(i)->getMeleeDetection().getGlobalBounds().intersects(m_player.getHB().getGlobalBounds()))
		{
			if (m_enemy.at(i)->getWeapon()->getMeleeWeaponAttackArea().intersects(m_player.getHB().getGlobalBounds()))
			{
				m_enemy.at(i)->attack(m_player.getPostion());
				if (m_enemy[i]->getWeapon()->attacking() && !m_player.isImmune)
				{
					if (m_enemy[i]->getAttackAnimationFrame() >= 5)
					{
						m_player.takeMeleeDamage(*m_enemy.at(i));
						bloodParticleSystem.addEmitter(BloodEmitter(m_player.getPostion()), explosionDuration); //makes a new particle emit at the enemy position
					}
					m_player.isImmune = true;
				}
				
			}
			else
			{
				m_enemy[i]->chase(m_player.getPostion());
			}	
		}
		else
		{
			if (m_enemy[i]->getBehaviourState() != BehaviourState::PathFinding)
			{
				m_enemy[i]->setVelocityVector({ 0,0 });
				m_enemy[i]->setBehaviourState(BehaviourState::None);
			}
			
		}
		//clear status of attacking since when an enemy attacks, we check above first and then set attacking to false, so we dont repeat takeDamage() function
		for (int i = 0; i < m_enemy.size(); i++)
		{
			if (m_enemy[i]->getWeapon()->getWeaponType() == WeaponType::Melee)
			{
				m_enemy[i]->getWeapon()->setAttacking(false);
			}
		}

	}
}

void GamePlay::increaseScore()
{
	if (m_enemiesKilled > scoreIncreaseThreshold)
	{
		scoreIncreaseThreshold *= 10;
		m_scoreIncreaseMultiplier += 0.1;
		
	}
	m_gameScore += scoreIncrement * m_scoreIncreaseMultiplier;
	myHud.updateIncrease(static_cast<int>(scoreIncrement * m_scoreIncreaseMultiplier));
}

void GamePlay::mouseUpdate()
{	
	for (int i = 0; i < m_enemy.size(); i++)
	{
		if (m_enemy[i]->getWeapon()->getWeaponType() == WeaponType::Ranged)
		{
			if (m_player.getHB().getGlobalBounds().intersects(m_enemy[i]->getRangedAttackRadius().getGlobalBounds()))
			{
				m_enemy[i]->attack(m_player.getPostion());
				m_enemy[i]->updateWeaponRotation(m_player.getPostion());
			}
		}
	}
	
	//m_enemy->attack(m_player.getPostion());
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_player.attack();
		myHud.setAmmoHudNumber(m_player.getAmmoCount());
		
		shake = true;
	}
	else
	{
		shake = false;
	}
}

/// <summary>
/// initialises the view for the game
/// </summary>
void GamePlay::initView()
{	
	camera.setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	camera.setCenter(m_player.getPostion());
	hudView.setSize(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	hudView.setCenter(m_player.getPostion());
}

/// <summary>
/// shakes camera in a 1-25 range from its centre when called
/// </summary>
void GamePlay::shakeCamera()
{
	if (shake)
	{
		camera.setCenter(camera.getCenter().x + rand() % 10, camera.getCenter().y + rand() % 10);  //range 1-5
	}
}
/// <summary>
/// Camera follows the player center
/// </summary>
void GamePlay::cameraFollow()
{
	camera.setCenter(m_player.getPostion());
	hudView.setCenter(m_player.getPostion());

	myHud.followCamera(m_player.getPostion());
}

/// <summary>
/// Checks all collision in game
/// </summary>
void GamePlay::checkCollision()
{
	checkPlayerBulletCollision();
	checkEnemyBulletCollision();
	checkAmmoPickupCollision();
}

/// <summary>
/// Specifically checks bullet to wall collisions
/// </summary>
void GamePlay::checkWallCollision(std::shared_ptr<ProjectilePool> t_projectilePool)
{
	std::array<Projectile, s_POOL_SIZE>& projectiles = t_projectilePool->getProjectiles();

	for (int j = 0; j < s_POOL_SIZE; j++)
	{
		if (projectiles[j].isOnScreen() == true) //check if bullet is on screen before all the wallss
		{
			for (int i = 0; i < collisionGrid.size(); i++) //uses collisions grid
			{
				if (projectiles.at(j).getBounds().intersects(collisionGrid.at(i)))
				{
					//std::cout << "I HIT A WALL";
					projectiles[j].setOnScreen(false); //bullet no longer on screen
					break;
				}
			}
		}
	}
}

//This is called from processInput for now
//Check wether a player is collding with a pickup and picks up new weapon and drops old one
void GamePlay::checkPickUpCollision()
{
	for (int i = 0; i < m_weaponPickups.size(); i++)
	{
		if (m_player.getHB().getGlobalBounds().intersects(m_weaponPickups[i].getGlobalBounds()) && !pickerUp)
		{
			std::cout << "picked up new Weapon";
			//playerDropWeapon();
			playerPickupWeapon(m_weaponPickups[i]);
			m_weaponPickups.erase(m_weaponPickups.begin() + i);
			pickerUp = true;
		}
	}
}

void GamePlay::checkPlayerBulletCollision()
{
	if (m_player.getWeaponArchetype() == WeaponType::Ranged)
	{
		std::shared_ptr<ProjectilePool>& projectilePool = m_player.getProjectilePool();
		std::array<Projectile, s_POOL_SIZE>& projectiles = projectilePool->getProjectiles();
		for (int i = 0; i < m_enemy.size(); i++)
		{
			if (m_enemy[i]->checkProjectileCollisions(projectilePool, m_player.getRangedWeaponType())) //checks if enemy has collision with any player bullets
			{
				m_enemy[i]->playHitSound();
				bloodParticleSystem.addEmitter(BloodEmitter(m_enemy[i]->getPosition()), explosionDuration); //makes a new particle emit at the enemy position
				increaseScore();
				m_enemy[i]->startChase = true;
			}
			checkWallCollision(projectilePool);
		}
	}
	if (m_player.getWeaponArchetype() == WeaponType::Melee)
	{
		for (int i = 0; i < m_enemy.size(); i++)
		{
			if (m_player.getWeapon()->attacking())
			{
				if (m_enemy[i]->checkMeleeCollisions(m_player.getMeleeWeaponAttack()) && m_enemy[i]->isImmune==false) //checks if melee area collides with enemy
				{
					m_enemy[i]->takeDamage(m_player.getMeleeWeaponType());
					bloodParticleSystem.addEmitter(BloodEmitter(m_enemy[i]->getPosition()), explosionDuration); //makes a new particle emit at the enemy position
					increaseScore();
					m_enemy[i]->playHitSound();
					m_enemy[i]->isImmune = true;
				}
			}
		}	
	}	
	if (m_player.getWeapon()->getWeaponType() == WeaponType::Melee)
	{
		m_player.getWeapon()->setAttacking(false);
	}
}

void GamePlay::checkEnemyBulletCollision()
{
	
	bool collision = false;

	for (int i = 0; i < m_enemy.size(); i++)
	{
		if (m_enemy[i]->getWeapon()->getWeaponType() == WeaponType::Ranged)
		{
			std::shared_ptr<ProjectilePool>& projectilePool = m_enemy[i]->getWeapon()->getProjectilePool(); //get projectile pool
			std::array<Projectile, s_POOL_SIZE>& projectiles = projectilePool->getProjectiles(); //get Projectiles

			checkWallCollision(projectilePool);
			for (unsigned int j = 0; j < projectiles.size(); j++) //loop through size of array
			{
				if (projectiles[j].isOnScreen() == true)
				{
					if (m_player.getHB().getGlobalBounds().intersects(projectiles[j].getBounds()))  //checks collision between sprite rectangles
					{
						projectiles[j].setOnScreen(false);
						collision = true; //if yes
						bloodParticleSystem.addEmitter(BloodEmitter(m_player.getPostion()), explosionDuration); //makes a new particle emit at the player position
						m_player.takeRangedDamage(*m_enemy[i]);
						break;
					}
				}
			}
		}
	}
	
	
}

void GamePlay::checkAmmoPickupCollision()
{
	for (int i = 0; i < m_ammoPickup.size(); i++)
	{
		if (m_player.getHB().getGlobalBounds().intersects(m_ammoPickup[i].getGlobalBounds()))
		{
			if (m_player.getWeaponArchetype() == WeaponType::Ranged)
			{
				switch (m_player.getRangedWeaponType())
				{
				case RangedWeaponTypes::Pistol:
					m_player.getWeapon()->receiveAmmo(4);
					break;
				case RangedWeaponTypes::Shotgun:
					m_player.getWeapon()->receiveAmmo(1);
					break;
				case RangedWeaponTypes::Rifle:
					m_player.getWeapon()->receiveAmmo(2);
					break;
				default:
					break;
				}
			}
			m_ammoPickup.erase(m_ammoPickup.begin() + i);
			myHud.setAmmoHudNumber(m_player.getAmmoCount());
		}
	}
}

void GamePlay::playerPickupWeapon(WeaponPickup t_pickup)
{
	int ammoCount = t_pickup.getAmmoCount();
	WeaponType weaponType = t_pickup.getWeaponArchetype();
	RangedWeaponTypes rangedType = RangedWeaponTypes::None;

	switch (weaponType)
	{
	case WeaponType::Ranged:
		rangedType = t_pickup.getRangedType();
		playerPickupRangedWeapon(rangedType, ammoCount);
		break;
	default:
		break;
	}
}


void GamePlay::playerPickupRangedWeapon(RangedWeaponTypes t_rangedType,int t_ammoCout)
{
	switch (t_rangedType)
	{
	case RangedWeaponTypes::Pistol:
		m_player.setRangedWeapon(m_pistolTexture, m_bulletTexture, t_rangedType, t_ammoCout);
		myHud.setGunHudTexture(m_pistolTexture);
		myHud.setAmmoHudNumber(t_ammoCout);
		break;
	case RangedWeaponTypes::Shotgun:
		m_player.setRangedWeapon(m_shotgunTexture, m_bulletTexture, t_rangedType, t_ammoCout);
		myHud.setGunHudTexture(m_shotgunTexture);
		myHud.setAmmoHudNumber(t_ammoCout);
		break;
	case RangedWeaponTypes::Rifle:
		m_player.setRangedWeapon(m_rifleTexture, m_bulletTexture, t_rangedType, t_ammoCout);
		myHud.setGunHudTexture(m_rifleTexture);
		myHud.setAmmoHudNumber(t_ammoCout);
		break;
	default:
		break;
	}
}

/// <summary>
/// Initialises textures for particles
/// </summary>
void GamePlay::initBlood()
{
	if (!texture.loadFromFile(BLOOD_SPRITE))  //loads blood sprite
	{
		std::cout << "error loading blood texture";
	}
	if (!dustTexture.loadFromFile(DUST_SPRITE)) //loads dust sprite
	{
		std::cout << "error loading blood texture";
	}

	bloodParticleSystem.setTexture(texture);
	bloodParticleSystem.addAffector(BloodAffector()); //adds affector for fading


	//dust particles
	dustParticleSystem.setTexture(dustTexture);
	dustParticleSystem.addAffector(DustAffector()); //adds affector for fading 

}
/// <summary>
/// makes dust particles follow the player
/// </summary>
void GamePlay::dustParticles()
{
	if (m_player.isMoving())  //checks if player is currently moving on screen
	{
		timer++;
		if (timer > 5)  //so partiles dont come too fast increase time for slower effect
		{
			dustParticleSystem.addEmitter(DustEmitter(m_player.getPostion(), mousePosition), explosionDuration); //emits new particle at player position
			timer = 0; //resets timer to start again
		}
	}
}


/// <summary>
/// checks collision between player and map
/// </summary>
void GamePlay::mapColl()
{
	for (int i = 0; i < collisionGrid.size(); i++)
	{
		if (m_player.getHB().getGlobalBounds().intersects(collisionGrid.at(i)))
		{
			m_player.stopVelocity();
		}
	}
	
}


void GamePlay::initMap()
{
	myGrid.createGrid(4480, 2560); //creates initial grid
	//myGrid.nodeFromWorldPoint(m_player.getPostion());
	//myGrid.writeNeighbours(4);

	for ( int i = 0; i < 80; i++) //80 is the amount of rows, 80 x 16 = total height of map
	{
		for (int j = 0; j < 140; j++) //140 is the amount of columns, 140x 16 = total width of map
		{
			if (mapCollisions[j + (i * 140)] == 628) //j will loop through columns, i * 140 will give us the row, 628 is the number where the collision box will be
			{
				myGrid.setImpassable(j*32, i*32); //sets immpasable in base grid for path finding
				collisionGrid.push_back(sf::FloatRect(j * 32, i * 32, 32, 32)); //adds to collision grid for collisions
			}
			if (mapCollisions[j + (i * 140)] == 629) //j will loop through columns, i * 140 will give us the row, 628 is the number where the collision box will be
			{
				spawnGrid.push_back(sf::FloatRect(j * 32, i * 32, 32, 32)); // loops for spawn location for enemies
			}
		}
	}

}

/// <summary>
/// makes enemies drop weapons if they have been killed
/// </summary>
void GamePlay::dropWeapon()
{
	for (int i = 0; i < m_enemy.size(); i++) //current enemies
	{
		if (m_enemy[i]->checkDeSpawn() && m_enemy[i]->getWeapon()->getWeaponType() == WeaponType::Ranged) //if those enemies are not alive
		{
			switch (m_enemy[i]->getWeapon()->getRangedWeaponType()) //check which ranged weapon they had
			{
			case RangedWeaponTypes::Pistol:
				enemyDropWeapon(m_enemy[i]->getWeapon()->getRangedWeaponType(), i);
				break;
			case RangedWeaponTypes::Shotgun:
				enemyDropWeapon(m_enemy[i]->getWeapon()->getRangedWeaponType(), i);  //go to drop function based on weapon type and enemy number
				break;
			case RangedWeaponTypes::Rifle:
				enemyDropWeapon(m_enemy[i]->getWeapon()->getRangedWeaponType(), i);
				break;
			}
		}
		if (m_enemy[i]->checkDeSpawn() && m_enemy[i]->getWeapon()->getWeaponType() == WeaponType::Melee) //if those enemies are not alive
		{
			m_ammoPickup.push_back(AmmoPickup(m_bulletTexture, m_enemy[i]->getPosition()));
		}
	}
}

/// <summary>
/// Drops the enemies weapon as an interactable pickup for the player
/// </summary>
/// <param name="t_types">ranged weapon type</param>
/// <param name="_enemyNum">enemy number</param>
void GamePlay::enemyDropWeapon(RangedWeaponTypes t_types, int _enemyNum)
{
	switch (t_types) //change based in type
	{
	case RangedWeaponTypes::Pistol:
		m_weaponPickups.push_back(WeaponPickup(m_pistolTexture, m_enemy[_enemyNum]->getPosition(), RangedWeaponTypes::Pistol, 15)); 
		break;
	case RangedWeaponTypes::Shotgun:
		m_weaponPickups.push_back(WeaponPickup(m_shotgunTexture, m_enemy[_enemyNum]->getPosition(), RangedWeaponTypes::Shotgun, 10)); //sets new weapon pick up with texture and ammo
		break;
	case RangedWeaponTypes::Rifle:
		m_weaponPickups.push_back(WeaponPickup(m_rifleTexture, m_enemy[_enemyNum]->getPosition(), RangedWeaponTypes::Rifle, 30));
		break;
	default:
		break;
	}
	
}
/// <summary>
/// sets enemy off screen if the enemy has died
/// </summary>
void GamePlay::killEnemy()
{
	for (int i = 0; i < m_enemy.size(); i++)
	{
		if (m_enemy[i]->checkDeSpawn())
		{
			m_enemiesKilled += 1;
			m_enemy.erase(m_enemy.begin() + i);
		}
	}
}

/// <summary>
/// gives pickups illusion of being animated
/// </summary>
void GamePlay::bouncePickUps()
{
	bounceTimer++;
	if (bounceTimer < 15)
	{
		bounce = { 0,-0.5 };
	}
	if (bounceTimer > 15)
	{
		bounce = { 0,0.5 };
	}
	if (bounceTimer > 30)
	{
		bounceTimer = 0;
	}
	for (int i = 0; i < m_weaponPickups.size(); i++)
	{
		m_weaponPickups[i].move(bounce);
	}
	for (int i = 0; i < m_ammoPickup.size(); i++)
	{
		m_ammoPickup[i].move(bounce);
	}
}


/// <summary>
/// Animates a circle getting bigger when the player finishes his dying animation
/// </summary>
void GamePlay::playCircle()
{
	deathCircle.setPosition(m_player.getPostion());
	deathCircleRadius+=15;
	deathCircle.setRadius(deathCircleRadius);
	deathCircle.setOrigin(deathCircleRadius, deathCircleRadius);

	if (deathCircleRadius > 600)
	{
		deathCircleRadius = 600;
		Menus::m_currentGameState = GameStates::Scoring;
		
	}
}

void GamePlay::setUpPlayerLight()
{
	playerLight.setRadius(80);
	playerLight.setOrigin(80, 80);
	playerLight.setFillColor(sf::Color(232, 232, 232, 78));
	

	darknessTexture.create(4480, 2560);

	darknessTexture.clear(DARK_OVERLAY);

	darknessTexture.draw(playerLight, sf::BlendNone);

	darknessTexture.display();

	overlay.setTexture(darknessTexture.getTexture());

}

void GamePlay::lightUpdate()
{
	playerLight.setPosition(m_player.getPostion());

}




