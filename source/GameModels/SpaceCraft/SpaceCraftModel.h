#pragma once
#include "Structs.h"
#include "GameModel.h"

class SpaceCraftModel : public GameModel
{
private:	
	
	
public:
	bool isBoosted = false;
	float colisionFactor = 0.02f;
	SpaceCraftModel();
	~SpaceCraftModel();
};