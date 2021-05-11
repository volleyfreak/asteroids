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
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "GameModels/GameModel.h"
#include <Asteroid/AsteroidModel.h>
#include <SpaceCraft/SpaceCraftModel.h>
#include <Bullet/BulletModel.h>
#include <Saucer/SaucerModel.h>
#include <Number/NumberModels.h>
#include <Sound/SoundController.h>
#include <View/AsteroidsView.h>

namespace asteroids {
	class AsteroidsController
	{
	private:
		unsigned int highscore = 0;
		bool shooted = false;
		unsigned int gameTick = 1;
		unsigned int soundPitch = 0;
		int waitForSpaceCraft = 401;
		bool gameIsRunning = true;

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

		void MoveSpaceCraft();
		void MoveOrCreateSaucer();
		void MoveOrCreateAsteroids();
		void MoveSpaceCraftBullets();
		void MoveSaucerBullets();
		void MoveExplosionParticles();
		void DrawHighScore();
		void DrawEndScreen();

		void UpdateInput(GLFWwindow* window, SpaceCraftModel& spaceCraft);
		void CreateExplosion(asteroids::Coords pos);
		int DestroySpaceCraft(SpaceCraftModel& spaceCraft);
		unsigned int DestroySaucer(SaucerModel* saucer);
		unsigned int SplitAsteroid(std::pair<AsteroidModel*, AsteroidsView*> asteroidPair, asteroids::Coords forward, float impact);
		bool CheckLifes(std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes);
		void collectDigits(std::vector<int>& digits, unsigned int num);
		
		std::pair<AsteroidModel*, AsteroidsView*> CreateAsteroid(int score, asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.025f, unsigned int killCount = 0, asteroids::Coords forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) });
		std::pair<SaucerModel*, AsteroidsView*> CreateBigSaucer();
		std::pair<SaucerModel*, AsteroidsView*> CreateSmallSaucer();
		std::pair<BulletModel*, AsteroidsView*> CreateBullet(asteroids::Coords pos, float rotation);
		std::pair<BulletModel*, AsteroidsView*> CreateSaucerBullet(asteroids::Coords pos);
		std::pair<BulletModel*, AsteroidsView*> CreateSmallSaucerBullet(asteroids::Coords pos, asteroids::Coords target);
	public:		
		AsteroidsController(GLFWwindow* w);
		~AsteroidsController();
		bool GameTick();
	};
}