#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "../GameModel.h"
#include "../source/Shader.h"
#include "Structs.h"

class AsteroidModel: public GameModel
{
private:

public:
	unsigned int killCount = 0;
	AsteroidModel(asteroids::Coords pos, float size, unsigned int killCount);
	~AsteroidModel();
};