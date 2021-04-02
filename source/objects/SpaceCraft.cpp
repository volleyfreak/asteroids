#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "SpaceCraft.h"
#include "../source/Renderer.h"

void updateInput(GLFWwindow* window, float forward, float rotation)
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

Position updatePosition(float forward, float rotation, Position pos)
{
	pos.x += forward * cos(rotation) - 0.0f * sin(rotation);
	pos.y += forward * sin(rotation) + 0.0f * cos(rotation);
	transferCoordinates(pos.x, pos.y);
	return pos;
}

SpaceCraft::SpaceCraft(GLFWwindow* w)
	:window(w), forward(0.1f), rotation(0.0f), pos({ 0.0f, 0.0f })
{
	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
	shader = CreateShader(source.VertexSource, source.FragmentSource);
}

SpaceCraft::~SpaceCraft()
{
}

VertexArray SpaceCraft::Bind() const
{
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray va;
	VertexBuffer vb(positions, 4 * 5 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);	
	return va;
}

void SpaceCraft::Unbind() const
{
	GLCall(glDeleteProgram(shader));
}

void SpaceCraft::GameTick(VertexArray va) const
{
	updateInput(window, forward, rotation);
	/* Render here */	

	GLCall(glUseProgram(shader));

	GLCall(const auto kSizeLocation = glGetUniformLocation(shader, "uSize"));
	GLCall(glUniform2f(kSizeLocation, 0.3f, 0.3f));

	Position rotatedPos = updatePosition(forward, rotation, pos);
	GLCall(const auto kPositionLocation = glGetUniformLocation(shader, "uPosition"));
	GLCall(glUniform2f(kPositionLocation, rotatedPos.x, rotatedPos.y));

	GLCall(const auto kRotateLocation = glGetUniformLocation(shader, "uRotate"));
	GLCall(glUniform1f(kRotateLocation, rotation));

	va.Bind();

	/*GLCall(const auto kRotationLocation = glGetUniformLocation(shader, "uRotation"));
	glUniform2f(kRotationLocation, a[0], yR);*/

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	GLCall(glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f));

	GLCall(glDrawArrays(GL_LINES, 0, 10));
}
