#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <list>
#include <set>
#include <iostream>
#include <memory>

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
	unsigned int waitForspaceShip = WAIT_FOR_SPACESHIP + 1;
	unsigned int waitedForAsteroids = WAIT_FOR_ASTEROIDS + 1;
	bool gameIsRunning = true;
	bool exitGame = false;
	bool escapeKeyDown = false;

	Shader shader;
	SpaceShipModel spaceShipModel;
	GLFWwindow* window;
	SoundController sound = SoundController();

	AsteroidsView spaceShipView;
	std::set<std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>>> asteroids;
	std::set<std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>>> bullets;
	std::set<std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>>> saucerBullets;
	std::set<std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>>> explosionBullets;
	std::set<std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>>> spaceShipExplosionBullets;
	std::set<std::pair<std::shared_ptr<SpaceShipModel>, std::shared_ptr<AsteroidsView>>> lifes;
	std::pair<std::shared_ptr<NumberModel>, std::shared_ptr<AsteroidsView>> numbers[10];
	std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> saucer;

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
	unsigned int DestroySaucer(std::shared_ptr<SaucerModel> saucer);
	unsigned int SplitAsteroid(std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>> asteroidPair, asteroids::Coords forward, float impact);
	bool CheckLifes(std::set<std::pair<std::shared_ptr<SpaceShipModel>, std::shared_ptr<AsteroidsView>>> lifes);
	void collectDigits(std::vector<int>& digits, unsigned int num);
	void resetPosition(SpaceShipModel& spaceShipModel);
		
	void CreateExplosion(asteroids::Coords pos);
	void CreatespaceShipExplosion(asteroids::Coords pos);
	std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>> CreateAsteroid(int score, asteroids::Coords pos = { asteroids::randomF(), asteroids::randomF() }, float size = 0.033f, unsigned int killCount = 0, asteroids::Coords forward = { asteroids::randomF(-0.005f), asteroids::randomF(0.005f) });
	std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> CreateBigSaucer();
	std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> CreateSmallSaucer();
	std::pair<std::shared_ptr<NumberModel>, std::shared_ptr<AsteroidsView>> CreateNumber(int number);
	std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> CreateBullet(asteroids::Coords pos, float rotation);
	std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> CreateSaucerBullet(asteroids::Coords pos);
	std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> CreateSmallSaucerBullet(asteroids::Coords pos, asteroids::Coords target);
public:		
	//AsteroidsController initialization
	// @param w Provide GLFWwindow
	AsteroidsController(GLFWwindow* w);
	~AsteroidsController();

	//main game loop
	bool GameTick();
};
