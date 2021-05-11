#pragma once
#include "GameModel.h"

class SpaceCraftModel : public GameModel
{
private:	
	
	
public:
	bool isBoosted = false;
	bool isActive = true;
	float colisionFactor = 0.02f;
	SpaceCraftModel();
	~SpaceCraftModel();
};