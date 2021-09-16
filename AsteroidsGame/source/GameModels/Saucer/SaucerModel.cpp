#include "SaucerModel.h"
#pragma once

void SaucerModel::Move()
{
}


void SaucerModel::ShootRandomBullet()
{
	float rotation = asteroids::randomF(M_PI);
	float x = 0.01f * cos(rotation) - 0.01f * sin(rotation);
	float y = 0.01f * sin(rotation) + 0.01f * cos(rotation);
	auto bullet = new BulletModel(pos, { x, y });
	this->bullets.insert(std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet))));
}

void SaucerModel::ShootTargetedBullet(asteroids::Coords target)
{
	float x = target.x - pos.x;
	float y = target.y - pos.y;
	asteroids::Coords forward = asteroids::normalize(x, y);
	auto bullet = new BulletModel(pos, forward);
	this->bullets.insert(std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet))));
}

SaucerModel::SaucerModel(int score, float size, float speed)
	: PhysicEngine()
{
	bufferSize = 40;
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
