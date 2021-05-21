#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <math.h>

#include "VertexArray.h"
#include "constants.h"
#include "Renderer.h"
#include "Controller/AsteroidsController.h"

#include <gtest/gtest.h>



int main(int argc, char* argv[])
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(ASTEROIDS_WIN_HEIGHT, ASTEROIDS_WIN_WIDTH, "Asteroids", NULL, NULL);
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
	
	AsteroidsController controller = AsteroidsController(window);
		
	std::cout << glGetString(GL_VERSION) << std::endl;

	bool gameIsRunning = true;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && gameIsRunning)
	{
		/* Render here */

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		gameIsRunning = controller.GameTick();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	exit(0);
	return 0;
}