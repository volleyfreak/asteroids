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
	float colisionFactor = 0.03f;
	SpaceCraftModel();
	~SpaceCraftModel();
};