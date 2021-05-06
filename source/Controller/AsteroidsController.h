#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include <cmath>
#include <list>
#include <set>
#include <iostream>
#include <windows.h>    //used for PlaySound function
#include <fstream>    //used for file handling

#include "constants.h"
#include "Structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "GameModels/GameModel.h"
#include <Asteroid/AsteroidModel.h>
#include <SpaceCraft/SpaceCraftModel.h>
#include <Bullet/BulletModel.h>
#include <Saucer/SaucerModel.h>
#include <Sound/SoundController.h>
#include <View/AsteroidsView.h>

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
	bool shooted = false;
	unsigned int gameTick = 1;
	int waitForSpaceCraft = 0;

	Shader shader;
	SpaceCraftModel spaceCraftModel;	
	GLFWwindow* window;
	SoundController sound = SoundController();

	AsteroidsView spaceCraftView;
	std::set<std::pair<AsteroidModel*, AsteroidsView*>> asteroids;
	std::set<std::pair<BulletModel*, AsteroidsView*>> bullets;
	std::set<std::pair<BulletModel*, AsteroidsView*>> saucerBullets;
	std::set<std::pair<BulletModel*, AsteroidsView*>> explosionBullets;
	std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes;
	std::pair<SaucerModel*, AsteroidsView*> saucer;

public:
	unsigned int highscore = 0;
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

	void UpdateInput(GLFWwindow* window, SpaceCraftModel& spaceCraft)
	{
		spaceCraft.forward.x *= 0.997f;
		spaceCraft.forward.y *= 0.997f;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			float x = 0.0001f * cos(spaceCraft.rotation);
			float y = 0.0001f * sin(spaceCraft.rotation);
			spaceCraft.forward.x += x;
			spaceCraft.forward.y += y;
			spaceCraft.isBoosted = true;
		}
		else {
			spaceCraft.isBoosted = false;
		}

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			spaceCraft.rotation -= 0.05f;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			spaceCraft.rotation += 0.05f;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			this->shooted = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!shooted) {
				CreateBullet(spaceCraftModel.pos, spaceCraft.rotation);
				shooted = true;
			}
			
		}
	}
	void CreateAsteroid(int score, asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.025f, unsigned int killCount = 0) {
		auto asteroid = new AsteroidModel(pos, size, killCount, score);
		asteroids.insert(std::make_pair(asteroid, new AsteroidsView(asteroid, shader)));
	}
	void CreateBigSaucer() {
		auto saucer = new SaucerModel(SCORE_LARGE_SAUCER);
		this->saucer = std::make_pair(saucer, new AsteroidsView(saucer, shader));
	}
	void CreateSmallSaucer() {
		auto saucer = new SaucerModel(SCORE_SMALL_SAUCER, 0.007f);
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
	
	void CreateExplosion(asteroids::Coords pos) {
		for (unsigned int i = 0; i < 20; i++) {
			float x = 0.002f * cos(asteroids::randomF(M_PI));
			float y = 0.002f * sin(asteroids::randomF(M_PI));
			auto bullet = new BulletModel(pos, { x, y });
			explosionBullets.insert(std::make_pair(bullet, new AsteroidsView(bullet, shader)));
		}
	}

	void CreateSaucerBullet(asteroids::Coords pos) {
		float x = 0.02f * cos(asteroids::randomF(M_PI));
		float y = 0.02f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		saucerBullets.insert(std::make_pair(bullet, new AsteroidsView(bullet, shader)));		
	}

	void SplitAsteroid(AsteroidModel* asteroid) {
		this->highscore += asteroid->score;
		this->sound.playAsteroidDestructionSound();
		this->CreateExplosion(asteroid->pos);
		if (asteroid->killCount < 2) {
			asteroid->killCount++;
			int score = SCORE_LARGE_ASTEROID;
			if (asteroid->killCount == 1) {
				score = SCORE_MEDIUM_ASTEROID;
			}
			if (asteroid->killCount == 2) {
				score = SCORE_SMALL_ASTEROID;
			}
			CreateAsteroid(score, asteroid->pos, asteroid->size * 0.4f, asteroid->killCount);
			CreateAsteroid(score, asteroid->pos, asteroid->size * 0.4f, asteroid->killCount);
		}
		std::cout << "current Score = " << this->highscore << std::endl;
	}

	void destroySpaceCraft() {
		this->CreateExplosion(this->spaceCraftModel.pos);
		this->lifes.erase(std::prev(this->lifes.end()));
		this->spaceCraftModel.forward = { 0.0f, 0.0f };
		this->spaceCraftModel.pos = { 0.0f, 0.0f };
		this->spaceCraftModel.rotation = 0.0f;		
		this->sound.playSpaceCraftDestructionSound();
		this->waitForSpaceCraft = -100;
	}
	
	void destroySaucer(SaucerModel* saucer) {
		this->highscore += saucer->score;
		saucer->isActive = false;
		this->CreateExplosion(saucer->pos);
		this->sound.playSaucerDestructionSound();
		std::cout << "current Score = " << this->highscore << std::endl;
	}
};