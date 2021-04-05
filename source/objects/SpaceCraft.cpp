#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "SpaceCraft.h"

void updateInput(GLFWwindow* window, float& forward, float& rotation)
{
	bool lockPosition;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		forward += 0.0001f;
		lockPosition = false;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		forward -= 0.0001f;
		lockPosition = false;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotation -= 0.05f;
		lockPosition = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotation += 0.05f;
		lockPosition = true;
	}
}

Position updatePosition(float& forward, float& rotation, Position& pos)
{
	pos.x += forward * cos(rotation) - 0.0f * sin(rotation);
	pos.y += forward * sin(rotation) + 0.0f * cos(rotation);
	transferCoordinates(pos.x, pos.y);
	return pos;
}

SpaceCraft::SpaceCraft(GLFWwindow* w, Shader& s, Position p)
	:window(w), shader{ s }, pos {p}
{
}

SpaceCraft::~SpaceCraft()
{
}

void SpaceCraft::Bind()
{
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexBuffer vb(positions, 4 * 5 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	shader.Bind();
}

void SpaceCraft::Unbind()
{
	va.Unbind();
	shader.Unbind();
}

void SpaceCraft::GameTick()
{
	updateInput(window, forward, rotation);
	updatePosition(forward, rotation, pos);
	
	shader.Bind(); 
	shader.SetUniform2f("uSize", 0.1f, 0.1f);
	shader.SetUniform2f("uPosition", pos.x, pos.y);
	shader.SetUniform1f("uRotate", rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();

	GLCall(glDrawArrays(GL_LINES, 0, 10));
}

