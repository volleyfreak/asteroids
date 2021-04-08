#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <math.h>

#include "VertexArray.h"
#include "constants/constants.h"
#include "Renderer.h"
#include "objects/SpaceCraft.h"
#include "objects/Asteroid.h"



int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(ASTEROIDS_WIN_HEIGHT, ASTEROIDS_WIN_WIDTH, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}
	Shader shader = Shader("res/shaders/basic.shader");

	srand((unsigned int)time(0));
	SpaceCraft spaceCraft = SpaceCraft(window, shader, { 0.0f, 0.0f });
	Asteroid asteroid = Asteroid(shader);
	Asteroid asteroid2 = Asteroid(shader);
	Asteroid asteroid3 = Asteroid(shader);
	Asteroid asteroid4 = Asteroid(shader);

	spaceCraft.Bind();
	asteroid.Bind();
	asteroid2.Bind();
	asteroid3.Bind();
	asteroid4.Bind();

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{	
		/* Render here */

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		spaceCraft.GameTick();
		asteroid.GameTick();
		asteroid2.GameTick();
		asteroid3.GameTick();
		asteroid4.GameTick();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}	

	glfwTerminate();
	return 0;
}