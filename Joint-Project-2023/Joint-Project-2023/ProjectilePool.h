#pragma once
#include "Projectile.h"
#include "Globals.h"
#include <array>

class ProjectilePool
{
public:
	ProjectilePool();
	void createProjectile(sf::Texture& t_textire, sf::Vector2f t_position, sf::Vector2f t_velocity);
	std::array<Projectile, s_POOL_SIZE> & getProjectiles() { return m_projectiles;}
	

	void update(sf::Time& t_deltaTime);
	void render(sf::RenderWindow& t_window);
private:
	std::array<Projectile, s_POOL_SIZE> m_projectiles;

	// The index of the next available projectile.
	int m_nextAvailable{ 0 };

	// A flag indicating whether there are any projectiles available (initially they are).
	bool m_poolFull{ false };
};

