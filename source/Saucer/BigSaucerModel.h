#pragma once


#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#include "../GameModel.h"
#include "../source/Shader.h"
#include "Structs.h"

class BigSaucerModel : public GameModel
{
private:

public:
	unsigned int ticks = 0;
	bool isActive = true;
	BigSaucerModel();
	~BigSaucerModel();
};