#pragma once

#include <SFML/Graphics.hpp>
#include <Thor/Particles/ParticleSystem.hpp>
#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Vectors/PolarVector2.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Time/CallbackTimer.hpp>
#include <array>
#include <memory>

const sf::Time	dustDuration = sf::seconds(0.2f);
const sf::Time	dustTailDuration = sf::seconds(0.5f);
const sf::Int64	tailsPerDust = 10;
const float		dustGravity = 30.f;

class DustEmitter
{
public:
	explicit DustEmitter(sf::Vector2f position, sf::Vector2f t_mousePos)
		: mAccumulatedTime(sf::Time::Zero)
		, mPosition(position)
		, mousePos(t_mousePos)
	{
	}

	void operator() (thor::EmissionInterface& system, sf::Time dt)
	{
		const sf::Time tailInterval = dustDuration / tailsPerDust;

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
		 

		 std::cout << mousePos.x  << "\n";

		 sf::Vector2f direction;

		 float rotation = 0.0f;
		 sf::Vector2f rotationVector = mousePos - mPosition;
		 rotation = (atan2(rotationVector.x, rotationVector.y)) * 180 / thor::Pi;
		 rotation = -rotation; //sfml window is upside down so negative rotation
		 rotation -= 90; //sprite facing rightways

		 thor::PolarVector2f velocity(100, thor::random(rotation-10, rotation+10));

		// Create particle at position of explosion, with emitter-specific color and at 80% initial scale
		thor::Particle particle(dustTailDuration);
		particle.position = mPosition;
		particle.scale *= 0.15f;
		particle.rotation = rand() % 90 + 1;

		// A tail contains 25 particles with different speeds and scales.
		// The largest particles move fastest, leading to a comet-like tail effect.
		for (sf::Int64 i = 0; i < 1; ++i)
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
	sf::Vector2f    mousePos;

};

class DustAffector
{
public:
	void operator() (thor::Particle& particle, sf::Time dt)
	{
		// Apply gravity, where particles with greater scale are affected stronger (quadratic influence)
		particle.velocity += dt.asSeconds() * sf::Vector2f(0.f, dustGravity) * particle.scale.x * particle.scale.y;

		// Let particles continuously fade out (particles with smaller scale have already lower alpha value at beginning)
		particle.color.a = static_cast<sf::Uint8>(256 * thor::getRemainingRatio(particle) * particle.scale.x);
	}
};