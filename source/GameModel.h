#pragma once
#include "Structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

class GameModel
{
private:

public:
	std::vector<float> positions;
	int bufferSize = 20;
	asteroids::Coords forward = { 0.0f, 0.0f };
	float rotation = 0.0f;
	float size = 0.0f;
	asteroids::Coords pos = { 0.0f, 0.0f };	
	float collisionFactor = 0.01f;
	GameModel(int positions) : bufferSize(positions) {}
	float GetCollisionRadius() {
		return this->size * this->collisionFactor;
	}
};