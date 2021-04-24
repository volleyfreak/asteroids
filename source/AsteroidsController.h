#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Structs.h"
#include "VertexBuffer.h"
#include <list>
#include <set>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "GameModel.h"
#include "../source/Shader.h"
#include <Asteroid/AsteroidModel.h>
#include <SpaceCraft/SpaceCraftModel.h>
#include <AsteroidsView.h>

class AsteroidsController 
{
	//struct Compare final
	//{
	//	bool operator()(std::pair<AsteroidModel, SpaceCraftView> a, std::pair<AsteroidModel, SpaceCraftView> b) const noexcept
	//	{
	//		return true; // comparision logic
	//	}
	//};
private:
	Shader shader;
	SpaceCraftModel spaceCraftModel;
	AsteroidsView spaceCraftView;
	std::set<std::pair<AsteroidModel*, AsteroidsView*>> asteroids;
	//Bullets bullets[];
	GLFWwindow* window;

public:
	AsteroidsController(GLFWwindow* w);
	~AsteroidsController();

	/*void Bind();
	void Unbind();*/

	bool GameTick();

	void transferCoordinates(float& x, float& y)
	{
		if (x > 1)
			x = -1;
		if (x < -1)
			x = 1;
		if (y > 1)
			y = -1;
		if (y < -1)
			y = 1;
	}	

	asteroids::Vector* updatePosition(asteroids::Vector* forward, asteroids::Vector* pos)
	{
		/*pos.x += forward.x * cos(rotation) - forward.y * sin(rotation);
		pos.y += forward.x * sin(rotation) + forward.y * cos(rotation);*/
		pos->x += forward->x;
		pos->y += forward->y;
		transferCoordinates(pos->x, pos->y);
		return pos;
	}

	bool isCollision(asteroids::Vector* pos1, float scale1, asteroids::Vector* pos2, float scale2) {
		//move coordinate system for collision detection

		float x = pos1->x - pos2->x;
		float y = pos1->y - pos2->y;

		float distance = sqrt((x * x) + (y * y));
		return distance <= scale1 + scale2;
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
};