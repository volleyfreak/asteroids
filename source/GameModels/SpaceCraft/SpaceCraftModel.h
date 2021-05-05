#pragma once
#include "Structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "GameModel.h"
#include "Shader.h"

class SpaceCraftModel : public GameModel
{
private:	
	
	
public:
	bool isBoosted = false;
	float colisionFactor = 0.03f;
	SpaceCraftModel();
	~SpaceCraftModel();
};