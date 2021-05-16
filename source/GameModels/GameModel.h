#pragma once
#include <vector>
#include "Structs.h"

class GameModel
{
private:
public:
	//todo: getter and setter
	std::vector<float> positions;
	int bufferSize = 0;
	asteroids::Coords forward = { 0.0f, 0.0f };
	float rotation = 0.0f;
	float size = 0.0f;
	asteroids::Coords pos = { 0.0f, 0.0f };	
	float collisionFactor = 0.01f;  
	GameModel() : bufferSize() {}
	float GetCollisionRadius() {
		return this->size * this->collisionFactor;
	}
};