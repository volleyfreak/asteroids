#pragma once

#include "../GameModel.h"
#include "Structs.h"

class AsteroidModel: public GameModel
{
private:

public:
	int score = 0;
	unsigned int killCount = 0;
	AsteroidModel(asteroids::Coords pos, float size, unsigned int killCount, int score);
	~AsteroidModel();
};