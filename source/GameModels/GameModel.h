#pragma once
#include <vector>
#include "Utils.h"

/**
 * Base class of the model implementation of the game
 *
 * Each GameObjectModel inherits of this class. With this pattern, there can be a generic view for all GameObjects
 *
 */
class GameModel
{
public:	
	int bufferSize = 0;
	float rotation = 0.0f;
	float size = 0.0f;
	float collisionFactor = 0.01f;
	std::vector<float> positions;	
	asteroids::Coords pos = { 0.0f, 0.0f };	
	asteroids::Coords forward = { 0.0f, 0.0f };
	
	GameModel() : bufferSize() {}
	~GameModel();

	// returns size * collisionFactor
	float GetCollisionRadius() {
		return this->size * this->collisionFactor;
	}
};