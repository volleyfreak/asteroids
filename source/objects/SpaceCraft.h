#pragma once
#include "BasicObject.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "../source/Shader.h"

class SpaceCraft: public asteroids::BasicObject
{
private:
	float positions[20] = {
		-0.25f, 0.2f,
		0.25f,  0.0f,

		-0.25f, -0.2f,
		0.25f,  0.0f,

		-0.25f,  -0.2f,
		-0.20f, -0.1f,

		-0.25f,  0.2f,
		-0.20f, 0.1f,

		-0.20f, 0.1f,
		-0.20f, -0.1f,
	};
	float forward = 0.0f;
	float rotation = 0.0f;
	asteroids::Position pos = { 0.0f, 0.0f };
	GLFWwindow* window;
	VertexArray va;
	Shader& shader;
	
public:
	SpaceCraft(GLFWwindow* w, Shader& s, asteroids::Position p);
	~SpaceCraft();

	void Bind();
	void Unbind();

	void GameTick();
};