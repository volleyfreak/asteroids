#pragma once

#include "../GameModel.h"

//Game object for all bullets or explosion particles
class BulletModel : public GameModel
{
public:
	unsigned int tickCount = 0;

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

