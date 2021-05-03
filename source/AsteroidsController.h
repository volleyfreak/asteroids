#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <cmath>
#include <list>
#include <set>
#include <iostream>
#include <windows.h>    //used for PlaySound function
#include <fstream>    //used for file handling

#include "constants/constants.h"
#include "Structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "GameModel.h"
#include "../source/Shader.h"
#include <Asteroid/AsteroidModel.h>
#include <SpaceCraft/SpaceCraftModel.h>
#include <Bullet/BulletModel.h>
#include <Saucer/SaucerModel.h>
#include <Sound/SoundController.h>
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
	unsigned int gameTick = 1;
private:
	Shader shader;
	SpaceCraftModel spaceCraftModel;
	AsteroidsView spaceCraftView;
	std::set<std::pair<AsteroidModel*, AsteroidsView*>> asteroids;
	std::set<std::pair<BulletModel*, AsteroidsView*>> bullets;
	std::set<std::pair<BulletModel*, AsteroidsView*>> saucerBullets;
	std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes;
	std::pair<SaucerModel*, AsteroidsView*> saucer;
	GLFWwindow* window;
	SoundController sound = SoundController();

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
				CreateBullet(spaceCraftModel.pos, rotation);
				shooted = true;
			}
			
		}
	}
	void CreateAsteroid(asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.05f, unsigned int killCount = 0) {
		auto asteroid = new AsteroidModel(pos, size, killCount);
		asteroids.insert(std::make_pair(asteroid, new AsteroidsView(asteroid, shader)));
	}
	void CreateBigSaucer() {
		auto saucer = new SaucerModel();
		this->saucer = std::make_pair(saucer, new AsteroidsView(saucer, shader));
	}
	void CreateSmallSaucer() {
		auto saucer = new SaucerModel(0.007f);
		this->saucer = std::make_pair(saucer, new AsteroidsView(saucer, shader));
	}

	void CreateBullet(asteroids::Coords pos, float rotation) {
		if (bullets.size() < 4) {
			float x = 0.02f * cos(rotation);
			float y = 0.02f * sin(rotation);		
			auto bullet = new BulletModel(pos, { x, y });
			bullets.insert(std::make_pair(bullet, new AsteroidsView(bullet, shader)));
		}
	}

	void CreateSaucerBullet(asteroids::Coords pos) {
		float x = 0.02f * cos(asteroids::randomF(M_PI));
		float y = 0.02f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		saucerBullets.insert(std::make_pair(bullet, new AsteroidsView(bullet, shader)));		
	}

	void SplitAsteroid(AsteroidModel* asteroid) {
		this->sound.playAsteroidDestructionSound();
		if (asteroid->killCount++ <= 1) {
			CreateAsteroid(asteroid->pos, asteroid->size * 0.5f, asteroid->killCount);
			CreateAsteroid(asteroid->pos, asteroid->size * 0.5f, asteroid->killCount);
		}
	}

	void destroySpaceCraft() {
		this->lifes.erase(std::prev(this->lifes.end()));
		this->spaceCraftModel.forward = { 0.0f, 0.0f };
		this->spaceCraftModel.pos = { 0.0f, 0.0f };
		this->spaceCraftModel.rotation = 0.0f;
		this->sound.playSpaceCraftDestructionSound();
	}
	
	void destroySaucer() {
		this->saucer.first->isActive = false;
		this->sound.playSaucerDestructionSound();
	}
};