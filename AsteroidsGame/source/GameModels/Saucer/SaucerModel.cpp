#include "SaucerModel.h"
#pragma once

void SaucerModel::ShootRandomBullet()
{
	float rotation = asteroids::randomF(M_PI);
	float x = 0.01f * cos(rotation) - 0.01f * sin(rotation);
	float y = 0.01f * sin(rotation) + 0.01f * cos(rotation);
	auto bullet = new BulletModel(position, { x, y });
	this->bullets.insert(std::shared_ptr<BulletModel>(bullet));
}

void SaucerModel::ShootTargetedBullet(asteroids::Coords target)
{
	float x = target.x - position.x;
	float y = target.y - position.y;
	asteroids::Coords forward = normalize(x, y);
	auto bullet = new BulletModel(position, forward);
	this->bullets.insert(std::shared_ptr<BulletModel>(bullet));
}

SaucerModel::SaucerModel(int score, float size, float speed)
	: PhysicEngine()
{
	bufferSize = 40;
	layout = {
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
	if (asteroids::randomF(0.005f) > 0.0f) {
		this->position = { 1.0f, asteroids::randomF() };
		speed *= -1;
	}
	else {
		this->position = { -1.0f, asteroids::randomF() };
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
