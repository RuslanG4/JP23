#include "ProjectilePool.h"

ProjectilePool::ProjectilePool()
{

}

void ProjectilePool::createProjectile(sf::Texture& t_textire, sf::Vector2f t_position, sf::Vector2f t_velocity)
{
	if (s_POOL_SIZE == m_nextAvailable)
	{
		m_poolFull = true;
	}

	// If no projectiles available, simply re-use the next in sequence.
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}

	m_projectiles.at(m_nextAvailable).init(t_textire,t_position,t_velocity);
	m_nextAvailable++;
}


void ProjectilePool::update(sf::Time& t_deltaTime)
{
	// The number of active projectiles.
	int activeCount = 0;
	// Assume the pool is not full initially.
	m_poolFull = false;
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		// If m_projectiles.at(i).update() returns true, then this projectile is onscreen
		// Note how the condition is negated (i.e. checking if projectile is not in use)
		if (!m_projectiles.at(i).update(t_deltaTime))
		{
			// If this projectile has expired, make it the next available.
			m_nextAvailable = i;
		}
		else
		{
			// So we know how many projectiles are active.
			activeCount++;
		}
	}
	// If no projectiles available, set a flag.
	if (s_POOL_SIZE == activeCount)
	{
		m_poolFull = true;
	}
}

void ProjectilePool::render(sf::RenderWindow& t_window)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i].render(t_window);
	}
}
