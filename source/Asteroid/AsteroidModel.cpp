#include "AsteroidModel.h"
#pragma once

AsteroidModel::AsteroidModel(float size)
	: GameModel(40)
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
	this->pos = { asteroids::randomF(), asteroids::randomF() };
	this->rotation = 0.0f;
	this->size = size;
	this->forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) };
}

AsteroidModel::~AsteroidModel()
{
}
