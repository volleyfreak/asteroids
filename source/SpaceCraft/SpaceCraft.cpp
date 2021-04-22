#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "SpaceCraft.h"


SpaceCraft::SpaceCraft(GLFWwindow* w, Shader& s, asteroids::Vector p)
	:window(w), shader{ s }, pos{ p }
{
}

SpaceCraft::~SpaceCraft()
{
}

void updateInput(GLFWwindow* window, asteroids::Vector& forward, float& rotation) //Controller
{
	forward.x *= 0.997f;
	forward.y *= 0.997f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		float x = 0.0001f * cos(rotation);
		float y = 0.0001f * sin(rotation);
		forward.x += x;
		forward.y += y;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotation -= 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotation += 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		//shoot bullet
	}
}

void SpaceCraft::Bind() //Darstellung
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
	pos = asteroids::BaseView::updatePosition(forward, pos);
	
	shader.Bind(); 
	shader.SetUniform2f("uSize", size, size);
	shader.SetUniform2f("uPosition", pos.x, pos.y);
	shader.SetUniform1f("uRotate", rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();
	GLCall(glDrawArrays(GL_LINES, 0, 10));
}

