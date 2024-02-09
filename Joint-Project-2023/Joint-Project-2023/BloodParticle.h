#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include <array>
#include <memory>

const sf::Time	explosionInterval = sf::seconds(1.f); //time between for automation, curretnly not used
const sf::Time	explosionDuration = sf::seconds(0.1f); //how long it lasts
const sf::Time	tailDuration = sf::seconds(0.15f);  //how far the tail goes and how long
const sf::Int64	tailsPerExplosion = 15;  //amount on particles per
const float		gravity = 30.f;

class BloodEmitter
{
public:
	explicit BloodEmitter(sf::Vector2f position)
		: mAccumulatedTime(sf::Time::Zero)
		, mPosition(position)
	{
	}

	void operator() (thor::EmissionInterface& system, sf::Time dt)
	{
		const sf::Time tailInterval = explosionDuration / tailsPerExplosion;

		// Accumulate passed time. If enough time has passed (tailInterval), emit a new tail and decrease accumulator.
		mAccumulatedTime += dt;
		while (mAccumulatedTime - tailInterval > sf::Time::Zero)
		{
			emitTail(system);
			mAccumulatedTime -= tailInterval;
		}
	}

private:
	void emitTail(thor::EmissionInterface& system)
	{
		// Create initial direction with random vector length and angle
		thor::PolarVector2f velocity(500, thor::random(0.f, 360.f));

		// Create particle at position of explosion, with emitter-specific color and at 80% initial scale
		thor::Particle particle(tailDuration);
		particle.position = mPosition;
		particle.scale *= 0.15f;
		particle.rotation = rand() % 360 + 1;

		// A tail contains 5 particles with different speeds and scales.
		// The largest particles move fastest, leading to a comet-like tail effect.
		for (sf::Int64 i = 0; i < 5; ++i)
		{
			// Decrease scale continuously
			particle.scale *= 0.95f;

			// Decrease speed continuously
			velocity.r *= 0.96f;
			particle.velocity = velocity;

			// Add adapted particle to particle system
			system.emitParticle(particle);
		}
	}

private:
	sf::Time		mAccumulatedTime;
	sf::Vector2f	mPosition;

};

// Custom affector that fades particles out and accelerates them according to scale
class BloodAffector
{
public:
	void operator() (thor::Particle& particle, sf::Time dt)
	{
		// Apply gravity, where particles with greater scale are affected stronger (quadratic influence)
		particle.velocity += dt.asSeconds() * sf::Vector2f(0.f, gravity) * particle.scale.x * particle.scale.y;

		// Let particles continuously fade out (particles with smaller scale have already lower alpha value at beginning)
		particle.color.a = static_cast<sf::Uint8>(256 * thor::getRemainingRatio(particle) * 0.8);
	}
};