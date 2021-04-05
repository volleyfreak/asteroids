#pragma once
#include "BasicObject.h"
#include "../source/Shader.h"

class SpaceCraft
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
	Position pos = { 0.0f, 0.0f };
	GLFWwindow* window;
	VertexArray va;
	Shader& shader;
	
public:
	SpaceCraft(GLFWwindow* w, Shader& s, Position p);
	~SpaceCraft();

	void Bind();
	void Unbind();

	void GameTick();
};