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
	SpaceCraft spaceCraft = SpaceCraft(window);
	spaceCraft.Bind();

	std::cout << glGetString(GL_VERSION) << std::endl;

	/* L oop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{	
		spaceCraft.GameTick();		
	}	

	glfwTerminate();
	return 0;
}