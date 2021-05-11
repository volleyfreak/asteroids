#pragma once

#include "../GameModel.h"
class NumberModels : public GameModel
{
private:
	std::vector<float> Get0();
	std::vector<float> Get1();
	std::vector<float> Get2();
	std::vector<float> Get3();
	std::vector<float> Get4();
	std::vector<float> Get5();
	std::vector<float> Get6();
	std::vector<float> Get7();
	std::vector<float> Get8();
	std::vector<float> Get9();
public:
	unsigned int tickCount = 0;
	NumberModels(int number, asteroids::Coords pos);
	~NumberModels();
};

