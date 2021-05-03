#include "SaucerModel.h"
#pragma once

SaucerModel::SaucerModel(float size)
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
	auto forward = asteroids::randomF(-0.005f);
	if (asteroids::randomF(0.005f) > 0.0f) {
		this->pos = { 1.0f, asteroids::randomF() };
		if (forward > 0.0f) {
			forward *= -1;
			forward += -0.002f;
		}
	}
	else {
		this->pos = { -1.0f, asteroids::randomF() };
		if (forward < 0.0f) {
			forward *= -1;
			forward += 0.002f;
		}
	}
	
	this->rotation = 0.0f;
	this->size = size;
	this->collisionFactor = 5.0f;
	this->forward = { forward, asteroids::randomF(0.005f) };
}

SaucerModel::~SaucerModel()
{
}
