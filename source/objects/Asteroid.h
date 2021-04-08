#pragma once


#include "BasicObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "../source/Shader.h"

class Asteroid: public asteroids::BasicObject
{
private:
	
	float positions[40] = {
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
	float forward = 0.0f;
	float rotation = 0.0f;
	asteroids::Position pos = { 0.0f, 0.0f };
	VertexArray va;
	Shader& shader;

public:
	Asteroid(Shader& s);
	~Asteroid();

	void Bind();
	void Unbind();

	void GameTick();
};