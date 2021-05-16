#include "SpaceCraftModel.h"



SpaceCraftModel::SpaceCraftModel()
	: GameModel()
{
	bufferSize = 28;
	positions = {
		-0.25f,  0.2f,
		 0.25f,  0.0f,

		-0.25f, -0.2f,
		 0.25f,  0.0f,

		-0.25f, -0.2f,
		-0.20f, -0.1f,

		-0.25f,  0.2f,
		-0.20f,  0.1f,

		-0.20f,  0.1f,
		-0.20f, -0.1f,

		-0.20f,  0.1f,
		-0.40f,  0.0f,

		-0.40f,  0.0f,
		-0.20f, -0.1f,
	};
	forward = { 0.0f, 0.0f };
	rotation = 0.0f;
	size = 0.1f;
	this->collisionFactor = 0.3f;
	pos = { 0.0f, 0.0f }; 
}

SpaceCraftModel::~SpaceCraftModel()
{
}
