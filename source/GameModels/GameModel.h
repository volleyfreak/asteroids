#pragma once
#include <vector>
#include "Structs.h"

class GameModel
{
private:
	/*class Foo
	{
		X x_;
	public:
		X x() const { return x_; }
		void x(X x) { x_ = std::move(x); }
	}*/
public:
	//todo: getter and setter
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