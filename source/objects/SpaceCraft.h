#pragma once
#include "BasicObject.h"

class SpaceCraft
{
private:
	
public:
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
	float forward;
	float rotation;
	Position pos;
	GLFWwindow* window;
	unsigned int shader;

	SpaceCraft(GLFWwindow* w);
	~SpaceCraft();

	VertexArray Bind() const;
	void Unbind() const;

	void GameTick(VertexArray va) const;
};