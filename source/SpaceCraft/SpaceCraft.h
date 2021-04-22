#pragma once
#include "../BaseView.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "../source/Shader.h"

class SpaceCraft: public asteroids::BaseView
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
	float rotation = 0.0f;
	asteroids::Vector forward = { 0.0f, 0.0f };
	GLFWwindow* window;
	VertexArray va;
	Shader& shader;
	
public:
	float size = 0.1f;
	asteroids::Vector pos = { 0.0f, 0.0f };
	SpaceCraft(GLFWwindow* w, Shader& s, asteroids::Vector p);
	~SpaceCraft();

	void Bind();
	void Unbind();

	void GameTick();
};