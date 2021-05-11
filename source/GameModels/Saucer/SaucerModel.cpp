#include "SaucerModel.h"
#pragma once

SaucerModel::SaucerModel(int score, float size, float speed)
	: GameModel(40)
{
	positions = {
		-5.0f,  0.0f,
		 5.0f,  0.0f,

		 5.0f,  0.0f,
		 3.0f, -1.5f,

		 3.0f, -1.5f,
		-3.0f, -1.5f,

		-3.0f, -1.5f,
		-5.0f,  0.0f,

		-5.0f,  0.0f,
		-3.0f,  1.5f,

		-3.0f,  1.5f,
		 3.0f,  1.5f,

		 3.0f,  1.5f,		
		 5.0f,  0.0f,

		-2.0f,  1.5f,
		-1.0f,  3.0f,

		-1.0f,  3.0f,
		 1.0f,  3.0f,

		 1.0f,  3.0f,
		 2.0f,  1.5f
	};
	if (asteroids::randomF(0.005f) > 0.0f) {
		this->pos = { 1.0f, asteroids::randomF() };
		speed *= -1;
	}
	else {
		this->pos = { -1.0f, asteroids::randomF() };
	}
	this->score = score;
	this->rotation = 0.0f;
	this->size = size;
	this->collisionFactor = 4.5f;
	this->forward = { speed , asteroids::randomF(0.005f) };
}

SaucerModel::~SaucerModel()
{
}
