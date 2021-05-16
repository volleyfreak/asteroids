#pragma once

#include "../GameModel.h"

class AsteroidModel: public GameModel
{
private:
	std::vector<float> GetAsteroid1();
	std::vector<float> GetAsteroid2();
	std::vector<float> GetAsteroid3();
public:
	int score = 0;
	unsigned int killCount = 0;
	AsteroidModel(asteroids::Coords pos, float size, unsigned int killCount, int score, asteroids::Coords forward);
	~AsteroidModel();
};