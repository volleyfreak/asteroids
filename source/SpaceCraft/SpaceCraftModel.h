#pragma once
#include "../Structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "GameModel.h"
#include "../source/Shader.h"

class SpaceCraftModel : public GameModel
{
private:	
	
	
public:
	unsigned int maxBulletCount = 4;
	unsigned int bulletCount = 0;
	SpaceCraftModel();
	~SpaceCraftModel();
};