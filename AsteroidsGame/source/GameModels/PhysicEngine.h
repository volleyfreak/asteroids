#pragma once
#include <vector>
#include "Utils.h"
#include <constants.h>

/**
 * Base class of the model implementation of the game
 *
 * Each GameObjectModel inherits of this class. With this pattern, there can be a generic view for all GameObjects
 *
 */
class PhysicEngine
{
private:
	// returns size * collisionFactor
	float GetCollisionRadius() {
		return this->size * this->collisionFactor;
	}

public:	
	int bufferSize = 0;
	float rotation = 0.0f;
	float size = 0.0f;
	float collisionFactor = 0.01f;

	std::vector<float> layout;	
	asteroids::Coords position = { 0.0f, 0.0f };	
	asteroids::Coords forward = { 0.0f, 0.0f };
	
	PhysicEngine() {};
	~PhysicEngine() {};

	/**
	 * Moves GameObject based on current position and moving vector
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	void Move()
	{
		this->position = UpdatePosition(this->forward, this->position);
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
		float x = this->position.x - otherModel.position.x;
		float y = this->position.y - otherModel.position.y;
		float distance = sqrt((x * x) + (y * y));
		return distance <= this->GetCollisionRadius() + otherModel.GetCollisionRadius();
	}

	/**
	 * Determines resulting moving vector of impact
	 *
	 * @param impact Size of impact
	 *
	 * @return resuling moving vector
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	asteroids::Coords CalculateImpact(float impact) {
		float rotation = abs(asteroids::randomF(M_PI / 4));
		float x = (this->forward.x * impact + this->forward.x) * cos(rotation);
		float y = (this->forward.y * impact + this->forward.y) * sin(rotation);
		return { x,y };
	}

	/**
	 * Sums up two vectors. Value is transvestited to other side if reaching end of screen.
	 *
	 * @param forward vector1
	 * @param forward vector2
	 *
	 * @return Sum of two vectors
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	asteroids::Coords UpdatePosition(const asteroids::Coords& forward, asteroids::Coords pos)
	{
		pos.x += forward.x;
		pos.y += forward.y;
		if (pos.x > 1)
			pos.x = -1;
		if (pos.x < -1)
			pos.x = 1;
		if (pos.y > 1)
			pos.y = -1;
		if (pos.y < -1)
			pos.y = 1;
		return pos;
	}

	/**
	 * Normalizes a vector to length 0.015f
	 *
	 * @param x, y coords of vector
	 *
	 * @return the normalized vector
	 *
	 */
	asteroids::Coords normalize(float x, float y)
	{
		float factor = 1 / sqrt((x * x) + (y * y));
		return { x * factor * 0.015f, y * factor * 0.015f };
	}
};