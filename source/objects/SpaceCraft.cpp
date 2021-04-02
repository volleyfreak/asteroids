#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "SpaceCraft.h"
#include "../source/Renderer.h"

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

SpaceCraft::SpaceCraft(GLFWwindow* w)
	:window(w), forward(0.1f), rotation(0.0f), pos({ 0.0f, 0.0f })
{
	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
	shader = CreateShader(source.VertexSource, source.FragmentSource);
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

	
	GLCall(glUseProgram(shader));

	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);	
}

void SpaceCraft::Unbind() const
{
	GLCall(glDeleteProgram(shader));
}

void SpaceCraft::GameTick()
{
	updateInput(window, forward, rotation);
	/* Render here */	

	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	GLCall(glUseProgram(shader));

	GLCall(const auto kSizeLocation = glGetUniformLocation(shader, "uSize"));
	GLCall(glUniform2f(kSizeLocation, 0.3f, 0.3f));

	updatePosition(forward, rotation, pos);
	GLCall(const auto kPositionLocation = glGetUniformLocation(shader, "uPosition"));
	GLCall(glUniform2f(kPositionLocation, pos.x, pos.y));

	GLCall(const auto kRotateLocation = glGetUniformLocation(shader, "uRotate"));
	GLCall(glUniform1f(kRotateLocation, rotation));


	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	GLCall(glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f));

	va.Bind();

	GLCall(glDrawArrays(GL_LINES, 0, 10));

	/* Swap front and back buffers */
	glfwSwapBuffers(window);

	/* Poll for and process events */
	glfwPollEvents();
}
