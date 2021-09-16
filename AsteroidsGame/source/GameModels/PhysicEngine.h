#pragma once
#include <vector>
#include "Utils.h"

/**
 * Base class of the model implementation of the game
 *
 * Each GameObjectModel inherits of this class. With this pattern, there can be a generic view for all GameObjects
 *
 */
class PhysicEngine
{
public:	
	int bufferSize = 0;
	float rotation = 0.0f;
	float size = 0.0f;
	float collisionFactor = 0.01f;
	std::vector<float> positions;	
	asteroids::Coords pos = { 0.0f, 0.0f };	
	asteroids::Coords forward = { 0.0f, 0.0f };
	
	PhysicEngine() {};
	~PhysicEngine() {};

	// returns size * collisionFactor
	float GetCollisionRadius() {
		return this->size * this->collisionFactor;
	}

	/**
	 * Determines collision of two objects
	 *
	 * @param pos1, pos2 Positions of the two objects
	 * @param scale1, scale2 Dimensions of the two objects
	 *
	 * @return boolean expression about collision
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	bool IsCollision(PhysicEngine otherModel)
	{
		float x = this->pos.x - otherModel.pos.x;
		float y = this->pos.y - otherModel.pos.y;
		float distance = sqrt((x * x) + (y * y));
		return distance <= this->GetCollisionRadius() + otherModel.GetCollisionRadius();
	}
};