#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <list>
#include <set>
#include <iostream>

#include "constants.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "GameModels/GameModel.h"
#include <Asteroid/AsteroidModel.h>
#include <SpaceShip/SpaceShipModel.h>
#include <Bullet/BulletModel.h>
#include <Saucer/SaucerModel.h>
#include <Number/NumberModel.h>
#include <Sound/SoundController.h>
#include <View/AsteroidsView.h>

/**
 * Controller implementation for the game
 *
 * Handels player input, modifies models and passes them to the view.
 *
 */
class AsteroidsController
{
private:
	unsigned int highscore = 0;
	bool shooted = false;
	unsigned int gameTick = 1;
	float soundPitch = 0.0f;
	int waitForspaceShip = WAIT_FOR_spaceShip + 1;
	bool gameIsRunning = true;
	bool exitGame = false;
	bool escapeKeyDown = false;

	Shader shader;
	SpaceShipModel spaceShipModel;
	GLFWwindow* window;
	SoundController sound = SoundController();

	AsteroidsView spaceShipView;
	std::set<std::pair<AsteroidModel*, AsteroidsView*>> asteroids;
	std::set<std::pair<BulletModel*, AsteroidsView*>> bullets;
	std::set<std::pair<BulletModel*, AsteroidsView*>> saucerBullets;
	std::set<std::pair<BulletModel*, AsteroidsView*>> explosionBullets;
	std::set<std::pair<BulletModel*, AsteroidsView*>> spaceShipExplosionBullets;
	std::set<std::pair<SpaceShipModel*, AsteroidsView*>> lifes;
	std::pair<NumberModel*, AsteroidsView*> numbers[10];
	std::pair<SaucerModel*, AsteroidsView*> saucer;

	void MoveSpaceShipAndUpdateInput();
	void MoveOrCreateSaucer();
	void MoveOrCreateAsteroids();
	void MoveSpaceShipBullets();
	void MoveSaucerBullets();
	void MoveExplosionParticles();
	void MoveSpaceShipExplosionParticles();
	void DrawHighScore();
	void DrawEndScreen();

	void UpdateInput(GLFWwindow* window, SpaceShipModel& spaceShip);
	int DestroySpaceShip(SpaceShipModel& spaceShip);
	unsigned int DestroySaucer(SaucerModel* saucer);
	unsigned int SplitAsteroid(std::pair<AsteroidModel*, AsteroidsView*> asteroidPair, asteroids::Coords forward, float impact);
	bool CheckLifes(std::set<std::pair<SpaceShipModel*, AsteroidsView*>> lifes);
	void collectDigits(std::vector<int>& digits, unsigned int num);
	void resetPosition(SpaceShipModel& spaceShipModel);
		
	void CreateExplosion(asteroids::Coords pos);
	void CreatespaceShipExplosion(asteroids::Coords pos);
	std::pair<AsteroidModel*, AsteroidsView*> CreateAsteroid(int score, asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.025f, unsigned int killCount = 0, asteroids::Coords forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) });
	std::pair<SaucerModel*, AsteroidsView*> CreateBigSaucer();
	std::pair<SaucerModel*, AsteroidsView*> CreateSmallSaucer();
	std::pair<NumberModel*, AsteroidsView*> CreateNumber(int number);
	std::pair<BulletModel*, AsteroidsView*> CreateBullet(asteroids::Coords pos, float rotation);
	std::pair<BulletModel*, AsteroidsView*> CreateSaucerBullet(asteroids::Coords pos);
	std::pair<BulletModel*, AsteroidsView*> CreateSmallSaucerBullet(asteroids::Coords pos, asteroids::Coords target);
public:		
	//AsteroidsController initialization
	// @param w Provide GLFWwindow
	AsteroidsController(GLFWwindow* w);
	~AsteroidsController();

	//main game loop
	bool GameTick();
};
