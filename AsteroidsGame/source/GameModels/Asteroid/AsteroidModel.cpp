#include "AsteroidModel.h"
#pragma once

AsteroidModel::AsteroidModel(asteroids::Coords pos, float size, unsigned int killCount, int score, asteroids::Coords forward)
	: PhysicEngine(), killCount(killCount)
{
	auto random = abs(asteroids::randomF(1.0f));
	if (random <= 0.333f) {
		positions = GetAsteroid1();
		bufferSize = 40;
	}
	else if (random <= 0.666f) {
		positions = GetAsteroid2();
		bufferSize = 44;
	}
	else {
		positions = GetAsteroid3();
		bufferSize = 44;
	}	
	this->score = score;
	this->pos = pos;
	this->rotation = 0.0f;
	this->size = size;
	this->collisionFactor = 2.7f;
	this->forward = forward;
}

AsteroidModel::~AsteroidModel()
{
}

std::vector<float> AsteroidModel::GetAsteroid1() {
	return {
		 0.0f,  2.0f,
		 1.5f,  3.0f,

		 1.5f,  3.0f,
		 3.0f,  2.0f,

		 3.0f,  2.0f,
		 2.0f,  0.0f,

		 2.0f,  0.0f,
		 3.0f, -2.0f,

		 3.0f, -2.0f,
		 1.0f, -3.0f,

		 1.0f, -3.0f,
		-1.0f, -3.0f,

		-1.0f, -3.0f,
		-3.0f, -1.0f,

		-3.0f, -1.0f,
		-3.0f,  2.0f,

		-3.0f,  2.0f,
		-1.5f,  3.0f,

		-1.5f,  3.0f,
		 0.0f,  2.0f
	};
}

std::vector<float> AsteroidModel::GetAsteroid2() {
	return {
		 1.5f,  3.0f,
		 3.0f,  0.75f,

		 3.0f,  0.75f,
		 3.0f, -0.75f,

		 3.0f, -0.75f,
		 1.5f, -3.0f,

		 1.5f, -3.0f,
		-0.5f, -3.0f,

		-0.5f, -3.0f,
		-0.5f, -1.0f,

		-0.5f, -1.0f,
		-2.0f, -3.0f,

		-2.0f, -3.0f,
		-3.0f, -0.5f,

		-3.0f, -0.5f,
		-1.5f,  0.0f,
			    
		-1.5f,  0.0f,
		-3.0f,  0.5f,
			    
		-3.0f,  0.5f,
		-1.0f,  3.0f,
			    
		-1.0f,  3.0f,
		 1.5f,  3.0f		
	};
}

std::vector<float> AsteroidModel::GetAsteroid3() {
	return {
		 1.0f,  2.5f,
		 3.0f,  1.0f,

		 3.0f,  1.0f,
		 0.5f,  0.0f,

		 0.5f,  0.0f,
		 3.0f,  -1.5f,

		 3.0f, -1.5f,
		 1.0f, -3.0f,

		 1.0f, -3.0f,
		 0.5f, -2.0f,

		 0.5f, -2.0f,
		-2.0f, -3.0f,

		-2.0f, -3.0f,
		-3.0f, -1.0f,

		-3.0f, -1.0f,
		-3.0f,  1.0f,

		-3.0f,  1.0f,
		-1.0f,  1.0f,

		-1.0f,  1.0f,
		-1.5f,  2.5f,

		-1.5f,  2.5f,
		 1.0f,  2.5f
	};
}

void AsteroidModel::Move()
{
	this->pos = asteroids::UpdatePosition(this->forward, this->pos);
}
