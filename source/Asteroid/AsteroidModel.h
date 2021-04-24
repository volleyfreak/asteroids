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
	
	/*float positions[40] = {
		0.0f, 2.0f, 
		1.5f, 3.0f,

		1.5f, 3.0f,
		3.0f, 2.0f,

		3.0f, 2.0f,
		2.0f, 0.0f,

		2.0f, 0.0f,
		3.0f, -2.0f,

		3.0f, -2.0f,
		1.0f, -3.0f,

		1.0f, -3.0f,
		-1.0f, -3.0f,
		
		-1.0f, -3.0f,
		-3.0f, -1.0f,

		-3.0f, -1.0f,
		-3.0f, 2.0f,

		-3.0f, 2.0f,
		-1.5f, 3.0f,

		-1.5f, 3.0f, 
		0.0f, 2.0f
	};
	asteroids::Vector forward = { 0.0f, 0.0f };
	float rotation = 0.0f;
	
	VertexArray va;
	Shader& shader;*/

public:
	/*asteroids::Vector pos = { 0.0f, 0.0f };
	float size = 0.03f;*/
	AsteroidModel(float size);
	~AsteroidModel();
};