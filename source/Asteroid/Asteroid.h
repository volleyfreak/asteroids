#pragma once


#include "BaseView.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "../source/Shader.h"

class Asteroid: public asteroids::BaseView
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
	asteroids::Vector forward = { 0.0f, 0.0f };
	float rotation = 0.0f;
	
	VertexArray va;
	Shader& shader;

public:
	asteroids::Vector pos = { 0.0f, 0.0f };
	float size = 0.03f;
	Asteroid(Shader& s, float direction);
	~Asteroid();

	void Bind();
	void Unbind();

	void GameTick();
};