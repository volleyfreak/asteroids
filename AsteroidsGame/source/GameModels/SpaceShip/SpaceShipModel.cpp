#include "SpaceShipModel.h"

std::shared_ptr<BulletModel> SpaceShipModel::CreateBullet(asteroids::Coords pos, float rotation)
{
	float x = 0.02f * cos(rotation);
	float y = 0.02f * sin(rotation);
	auto bullet = new BulletModel(pos, { x, y });
	return std::shared_ptr<BulletModel>(bullet);
}

void SpaceShipModel::shoot()
{
	if (!this->shooted && this->bullets.size() < 4) {
		this->bullets.insert(CreateBullet(this->position, this->rotation));
		this->shooted = true;
	}
}

SpaceShipModel::SpaceShipModel()
	: PhysicEngine()
{
	bufferSize = 28;
	layout = {
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
	position = { 0.0f, 0.0f }; 
}

SpaceShipModel::~SpaceShipModel()
{
}
