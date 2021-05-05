#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "../GameModel.h"
#include "Shader.h"
#include "Structs.h"

class SaucerModel : public GameModel
{
private:
	
public:
	unsigned int ticks = 0;
	int score = 0;
	bool isActive = true;
	SaucerModel(int score, float size = 0.01f);
	~SaucerModel();
};