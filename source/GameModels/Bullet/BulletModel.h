#pragma once

#include "../GameModel.h"
class BulletModel : public GameModel
{
public:
	unsigned int tickCount = 0;
	BulletModel(asteroids::Coords pos, asteroids::Coords forward);
	~BulletModel();
};

