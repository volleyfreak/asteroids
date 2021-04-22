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
#include "SpaceCraft/SpaceCraft.h"
#include "Asteroid/Asteroid.h"
#include "BaseView.h"



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
	Shader shader = Shader("res/shaders/basic.shader");

	srand((unsigned int)time(0));
	SpaceCraft spaceCraft = SpaceCraft(window, shader, { 0.0f, 0.0f });
	Asteroid asteroid1 = Asteroid(shader, 0.004f);
	Asteroid asteroid2 = Asteroid(shader, -0.004f);
	Asteroid asteroid3 = Asteroid(shader, 0.008f);
	Asteroid asteroid4 = Asteroid(shader, -0.008f);

	Asteroid asteroids[4] = { asteroid1, asteroid2, asteroid3, asteroid4 };

	spaceCraft.Bind();
	for (Asteroid& asteroid : asteroids) {		
		asteroid.Bind();
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	bool gameIsRunning = true;
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && gameIsRunning)
	{	
		/* Render here */

		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		spaceCraft.GameTick();

		for (Asteroid& asteroid: asteroids){
			asteroid.GameTick();
			if (asteroids::isCollision(spaceCraft.pos, 0.01f, asteroid.pos, 0.1f)) {
				//asteroid.Unbind();
				//gameIsRunning = false;
				std::cout << "spacecraft: " << spaceCraft.pos.x << " y: " << spaceCraft.pos.y << "\n Asteroid: " << asteroid.pos.x << " " << asteroid.pos.y << ":" << std::endl;
			}			
		}		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}	

	glfwTerminate();
	return 0;
}