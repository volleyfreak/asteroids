#include "BulletModel.h"

BulletModel::BulletModel()
	: GameModel(2)
{
	positions = {
		0.0f, 0.0f,
	};
	this->pos = { asteroids::randomF(), asteroids::randomF() };
	this->rotation = 0.0f;
	this->size = size;
	this->forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) };
}
BulletModel::~BulletModel()
{
}
