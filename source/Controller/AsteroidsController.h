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
	
	void UpdateInput(GLFWwindow* window, SpaceCraftModel& spaceCraft);
	void CreateExplosion(asteroids::Coords pos);
	int DestroySpaceCraft(SpaceCraftModel* spaceCraft);
	unsigned int DestroySaucer(SaucerModel* saucer);
	unsigned int SplitAsteroid(std::pair<AsteroidModel*, AsteroidsView*> asteroidPair);
	bool CheckLifes(std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes);


	std::pair<AsteroidModel*, AsteroidsView*> CreateAsteroid(int score, asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.025f, unsigned int killCount = 0) {
		auto asteroid = new AsteroidModel(pos, size, killCount, score);
		return std::make_pair(asteroid, new AsteroidsView(asteroid, shader));
	}
	std::pair<SaucerModel*, AsteroidsView*> CreateBigSaucer() {
		auto saucer = new SaucerModel(SCORE_LARGE_SAUCER);
		return std::make_pair(saucer, new AsteroidsView(saucer, shader));
	}
	std::pair<SaucerModel*, AsteroidsView*> CreateSmallSaucer() {
		auto saucer = new SaucerModel(SCORE_SMALL_SAUCER, 0.007f);
		return std::make_pair(saucer, new AsteroidsView(saucer, shader));
	}

	std::pair<BulletModel*, AsteroidsView*> CreateBullet(asteroids::Coords pos, float rotation) {
		float x = 0.02f * cos(rotation);
		float y = 0.02f * sin(rotation);
		auto bullet = new BulletModel(pos, { x, y });
		return std::make_pair(bullet, new AsteroidsView(bullet, shader));
	}

	std::pair<BulletModel*, AsteroidsView*> CreateSaucerBullet(asteroids::Coords pos) {
		float x = 0.02f * cos(asteroids::randomF(M_PI));
		float y = 0.02f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		return std::make_pair(bullet, new AsteroidsView(bullet, shader));
	}	
};