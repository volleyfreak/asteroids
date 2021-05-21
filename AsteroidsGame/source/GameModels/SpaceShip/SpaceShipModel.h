#pragma once
#include "GameModel.h"

//Main playing figure, a spaceship that is moved by player input and can shoot
class SpaceShipModel : public GameModel
{
public:
	bool isBoosted = false;
	bool isActive = true;
	SpaceShipModel();
	~SpaceShipModel();
};