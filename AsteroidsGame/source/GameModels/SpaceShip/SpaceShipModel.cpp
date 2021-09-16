#include "SpaceShipModel.h"



void SpaceShipModel::Move()
{
	this->pos = asteroids::UpdatePosition(this->forward, this->pos);
}

std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> SpaceShipModel::CreateBullet(asteroids::Coords pos, float rotation)
{
	float x = 0.02f * cos(rotation);
	float y = 0.02f * sin(rotation);
	auto bullet = new BulletModel(pos, { x, y });
	return std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet)));
}

void SpaceShipModel::shoot()
{
	if (!this->shooted && this->bullets.size() < 4) {
		this->bullets.insert(CreateBullet(this->pos, this->rotation));
		this->shooted = true;
	}
}

SpaceShipModel::SpaceShipModel()
	: PhysicEngine()
{
	bufferSize = 28;
	positions = {
		-0.25f,  0.2f,
		 0.30f,  0.0f,

		-0.25f, -0.2f,
		 0.30f,  0.0f,

		-0.25f, -0.2f,
		-0.15f, -0.1f,

		-0.25f,  0.2f,
		-0.15f,  0.1f,

		-0.15f,  0.1f,
		-0.15f, -0.1f,

		-0.15f,  0.1f,
		-0.40f,  0.0f,

		-0.40f,  0.0f,
		-0.15f, -0.1f,
	};
	forward = { 0.0f, 0.0f };
	rotation = 0.0f;
	size = 0.1f;
	this->collisionFactor = 0.3f;
	pos = { 0.0f, 0.0f }; 
}

SpaceShipModel::~SpaceShipModel()
{
}
