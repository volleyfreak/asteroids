#include "BigSaucerModel.h"
#pragma once

BigSaucerModel::BigSaucerModel()
	: GameModel(40)
{
	positions = {
		-6.0f,  0.0f,
		 6.0f,  0.0f,

		 6.0f,  0.0f,
		 3.0f, -1.5f,

		 3.0f, -1.5f,
		-3.0f, -1.5f,

		-3.0f, -1.5f,
		-6.0f,  0.0f,

		-6.0f,  0.0f,
		-3.0f,  1.5f,

		-3.0f,  1.5f,
		 3.0f,  1.5f,

		 3.0f,  1.5f,		
		 6.0f,  0.0f,

		-2.0f,  1.5f,
		-1.0f,  3.0f,

		-1.0f,  3.0f,
		 1.0f,  3.0f,

		 1.0f,  3.0f,
		 2.0f,  1.5f
	};
	this->pos = { asteroids::randomF(), asteroids::randomF() };
	this->rotation = 0.0f;
	this->size = 0.01f;
	this->forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) };
}

BigSaucerModel::~BigSaucerModel()
{
}
