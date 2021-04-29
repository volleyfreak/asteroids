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
#include <Bullet/BulletModel.h>
#include <Saucer/BigSaucerModel.h>
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
	bool shooted = false;
private:
	Shader shader;
	SpaceCraftModel spaceCraftModel;
	AsteroidsView spaceCraftView;
	std::set<std::pair<AsteroidModel*, AsteroidsView*>> asteroids;
	std::set<std::pair<BulletModel*, AsteroidsView*>> bullets;
	std::pair<BigSaucerModel*, AsteroidsView*> saucer;
	GLFWwindow* window;

public:
	AsteroidsController(GLFWwindow* w);
	~AsteroidsController();

	bool GameTick();

	//todo: move functions to cpp file
	void TransferCoordinates(float& x, float& y);

	asteroids::Coords* UpdatePosition(asteroids::Coords* forward, asteroids::Coords* pos)
	{
		/*pos.x += forward.x * cos(rotation) - forward.y * sin(rotation);
		pos.y += forward.x * sin(rotation) + forward.y * cos(rotation);*/
		pos->x += forward->x;
		pos->y += forward->y;
		TransferCoordinates(pos->x, pos->y);
		return pos;
	}

	bool IsCollision(asteroids::Coords pos1, float scale1, asteroids::Coords pos2, float scale2) {
		//move coordinate system for collision detection

		float x = pos1.x - pos2.x;
		float y = pos1.y - pos2.y;

		float distance = sqrt((x * x) + (y * y));
		return distance <= scale1 + scale2;
	}

	void UpdateInput(GLFWwindow* window, asteroids::Coords& forward, float& rotation) 
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

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			this->shooted = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!shooted) {
				spaceCraftModel.bulletCount = CreateBullet(spaceCraftModel.pos, rotation, spaceCraftModel.bulletCount, spaceCraftModel.maxBulletCount);
				shooted = true;
			}
			
		}
	}
	void CreateAsteroid(asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.05f, unsigned int killCount = 0) {
		auto asteroid = new AsteroidModel(pos, size, killCount);
		asteroids.insert(std::make_pair(asteroid, new AsteroidsView(asteroid, shader)));
	}

	unsigned int CreateBullet(asteroids::Coords pos, float rotation, unsigned int bulletCount = 0, unsigned int maxBulletCount = 1) {
		if (bulletCount < maxBulletCount) {
			float x = 0.01f * cos(rotation);
			float y = 0.01f * sin(rotation);
		
			auto bullet = new BulletModel(pos, { x, y });
			bullets.insert(std::make_pair(bullet, new AsteroidsView(bullet, shader)));
		}
		return bulletCount++;
	}
	void SplitAsteroid(AsteroidModel* asteroid) {
		if (asteroid->killCount++ <= 1) {
			CreateAsteroid(asteroid->pos, asteroid->size * 0.5f, asteroid->killCount);
			CreateAsteroid(asteroid->pos, asteroid->size * 0.5f, asteroid->killCount);
		}
	}
};