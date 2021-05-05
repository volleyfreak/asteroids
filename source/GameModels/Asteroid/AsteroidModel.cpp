#include "AsteroidModel.h"
#pragma once

AsteroidModel::AsteroidModel(asteroids::Coords pos, float size, unsigned int killCount, int score)
	: GameModel(40), killCount(killCount)
{
	bufferSize = 40;
	positions = {
		0.0f, 2.0f,
		1.5f, 3.0f,

		1.5f, 3.0f,
		3.0f, 2.0f,

		3.0f, 2.0f,
		2.0f, 0.0f,

		2.0f, 0.0f,
		3.0f, -2.0f,

		3.0f, -2.0f,
		1.0f, -3.0f,

		1.0f, -3.0f,
		-1.0f, -3.0f,

		-1.0f, -3.0f,
		-3.0f, -1.0f,

		-3.0f, -1.0f,
		-3.0f, 2.0f,

		-3.0f, 2.0f,
		-1.5f, 3.0f,

		-1.5f, 3.0f,
		0.0f, 2.0f
	};
	this->score = score;
	this->pos = pos;
	this->rotation = 0.0f;
	this->size = size;
	this->collisionFactor = 2.5f;
	this->forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) };
}

AsteroidModel::~AsteroidModel()
{
}
