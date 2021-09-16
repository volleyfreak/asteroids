#pragma once

#include "../PhysicEngine.h"

//Game object for all bullets or explosion particles
class BulletModel : public PhysicEngine
{
public:
	unsigned int tickCount = 0;

	void Move();

	/**
	 * Constructor for BulletModel game object
	 *
	 * @param pos Position where to draw object
	 * @param forward Vector for movement
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	BulletModel(asteroids::Coords pos, asteroids::Coords forward);
	~BulletModel();
};

