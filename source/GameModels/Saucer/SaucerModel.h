#pragma once

#include "../GameModel.h"

class SaucerModel : public GameModel
{
private:
	
public:
	unsigned int ticks = 0;
	int score = 0;
	bool isActive = true;
	bool isSmallSaucer = false;
	SaucerModel(int score, float size = 0.01f, float speed = 0.003f);
	~SaucerModel();
};