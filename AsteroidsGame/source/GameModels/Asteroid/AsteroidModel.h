#pragma once

#include "../PhysicEngine.h"

// Game object class for all 3 shapes of Asteroids. They shape is selected randomly.
class AsteroidModel: public PhysicEngine
{
private:
	std::vector<float> GetAsteroid1();
	std::vector<float> GetAsteroid2();
	std::vector<float> GetAsteroid3();
public:
	unsigned int score = 0;
	unsigned int killCount = 0;

	/**
	 * Constructor for AsteroidModel game object
	 *
	 * @param pos Position where to draw object
	 * @param size Size of asteroids
	 * @param killCount Counter for determining if asteroid can split
	 * @param score Score for destroying asteroid
	 * @param forward Vector for movement
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	AsteroidModel(asteroids::Coords pos, float size, unsigned int killCount, int score, asteroids::Coords forward);
	~AsteroidModel();
};