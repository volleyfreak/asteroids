#include "BulletModel.h"

BulletModel::BulletModel(asteroids::Coords pos, asteroids::Coords forward)
	: PhysicEngine()
{
	bufferSize = 2;
	layout = {
		0.0f, 0.0f,
		0.0f, 0.05f,
	};
	this->position = pos;
	this->rotation = 0.0f;
	this->size = 1.0f;
	this->collisionFactor = 0.01f;
	this->forward = forward;
}
BulletModel::~BulletModel()
{
}