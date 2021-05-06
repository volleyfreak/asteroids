#include "AsteroidsController.h"



AsteroidsController::AsteroidsController(GLFWwindow* w)
	:shader(Shader("res/shaders/basic.shader")), 
	spaceCraftModel(SpaceCraftModel()),
	spaceCraftView(AsteroidsView(&spaceCraftModel, shader)),
	window(w)
{
	srand(time(NULL));
	for (auto i = 0; i < 4; i++) {
		auto life = new SpaceCraftModel();
		life->rotation = M_PI / 2;
		lifes.insert(std::make_pair(life, new AsteroidsView(life, shader)));
	}

	for (auto i = 0; i < 3; i++) {
		this->asteroids.insert(CreateAsteroid(SCORE_LARGE_ASTEROID));
	}
	this->saucer = this->CreateBigSaucer();
	this->saucer.first->isActive = false;
}

AsteroidsController::~AsteroidsController()
{
}

bool AsteroidsController::GameTick()
{
	this->sound.playBackgroundSound();
	//draw lifes
	if (this->CheckLifes(this->lifes)) {
		return true;
	}

	// spacecraft moves
	if (this->waitForSpaceCraft > 0) {
		UpdateInput(window, this->spaceCraftModel);
		this->spaceCraftModel.pos = UpdatePosition(this->spaceCraftModel.forward, this->spaceCraftModel.pos);

		if (this->spaceCraftModel.isBoosted && this->gameTick % 2 == 0) {
			this->spaceCraftView.GameTick(this->spaceCraftModel, this->spaceCraftModel.pos, 14);
		}
		else {
			this->spaceCraftView.GameTick(this->spaceCraftModel, this->spaceCraftModel.pos, 10);
		}
	}
	else {
		this->waitForSpaceCraft++;
	}

	// saucer moves if exists
	if (this->saucer.first->isActive) { 
		this->saucer.first->pos = UpdatePosition(this->saucer.first->forward, this->saucer.first->pos);
		this->saucer.second->GameTick(*this->saucer.first, this->saucer.first->pos, 20);
		if (this->saucer.first->ticks++ % 100 == 0) {
			CreateSaucerBullet(this->saucer.first->pos);
		}
		if (this->saucer.first->ticks++ % 133 == 0) {
			this->saucer.first->forward.y = asteroids::randomF(-0.005f);
		}
		if (abs(this->saucer.first->pos.x) == 1.0f) {
			this->saucer.first->isActive = false;
			this->sound.switchToGameSound();
		}
		if (IsCollision(this->saucer.first->pos, this->saucer.first->GetCollisionRadius(), spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius())) {
			this->highscore += this->DestroySaucer(this->saucer.first);
			this->waitForSpaceCraft = this->DestroySpaceCraft(&this->spaceCraftModel);
		  }
	}

	else {
		this->gameTick++;
		if (this->gameTick % 3000 == 0) {
			this->saucer = this->CreateSmallSaucer();
			this->sound.switchToSaucerSound();
		}
		else if (this->gameTick % 1000 == 0) {
			this->saucer = this->CreateBigSaucer();
			this->sound.switchToSaucerSound();
		}
	}	

	//asteroids move
	if (this->asteroids.size() == 0) {
		for (auto i = 0; i < 3; i++) {
			this->asteroids.insert(this->CreateAsteroid(SCORE_LARGE_ASTEROID));
		}
	}
	else {
		for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
		{
			auto asteroid = *it++;
			asteroid.first->pos = UpdatePosition(asteroid.first->forward, asteroid.first->pos);
			asteroid.second->GameTick(*asteroid.first, asteroid.first->pos, asteroid.first->bufferSize / 2);

			if (IsCollision(spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				this->highscore += this->SplitAsteroid(asteroid);
				this->waitForSpaceCraft = this->DestroySpaceCraft(&this->spaceCraftModel);
			}
			if (this->saucer.first->isActive && IsCollision(saucer.first->pos, this->saucer.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				this->highscore += this->SplitAsteroid(asteroid);
				this->highscore += this->DestroySaucer(this->saucer.first);
			}
		}
	}	

	//bullets collision
	for (auto bulletIt = this->bullets.begin(); bulletIt != this->bullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->BulletTick(*bullet.first, bullet.first->pos, 1);
		if (bullet.first->tickCount++ > 100) {
			this->bullets.erase(bullet);
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it++;
				if (IsCollision(bullet.first->pos, bullet.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
					this->bullets.erase(bullet);
					this->highscore += this->SplitAsteroid(asteroid);
				}
			}
			if (this->saucer.first->isActive && IsCollision(saucer.first->pos, saucer.first->GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
				this->bullets.erase(bullet);
				this->highscore += this->DestroySaucer(this->saucer.first);

				std::cout << "current Score = " << this->highscore << std::endl;
			}
		}		
	}
	
	//saucerBulletsMove
	for (auto bulletIt = this->saucerBullets.begin(); bulletIt != this->saucerBullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->BulletTick(*bullet.first, bullet.first->pos, 1);
		if (bullet.first->tickCount++ > 100) {
			this->saucerBullets.erase(bullet);
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it++;
				if (IsCollision(bullet.first->pos, bullet.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
					this->saucerBullets.erase(bullet);
					this->highscore += this->SplitAsteroid(asteroid);

				}
			}
			if (IsCollision(spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
				this->saucerBullets.erase(bullet);
				this->waitForSpaceCraft = this->DestroySpaceCraft(&this->spaceCraftModel);
			}
		}		
	}	

	//explosion bullets move
	for (auto bulletIt = this->explosionBullets.begin(); bulletIt != this->explosionBullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->BulletTick(*bullet.first, bullet.first->pos, 1);
		if (bullet.first->tickCount++ > 50) {
			this->explosionBullets.erase(bullet);
		}
	}
	return true;
}

bool AsteroidsController::CheckLifes(std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes) {
	if (lifes.size() == 0) {
		return true;
	}
	float xPos = 0;
	for (auto lifeIt = lifes.begin(); lifeIt != lifes.end(); )
	{
		auto life = *lifeIt++;
		life.second->GameTick(*life.first, { -0.95f + xPos, 0.95f }, life.first->bufferSize / 2 - 4);
		xPos += 0.07f;
	}
	return false;
}

void AsteroidsController::UpdateInput(GLFWwindow* window, SpaceCraftModel& spaceCraft)
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
		if (!this->shooted && this->bullets.size() < 4) {
			this->bullets.insert(CreateBullet(spaceCraftModel.pos, spaceCraft.rotation));
			this->shooted = true;
		}

	}
}

void AsteroidsController::CreateExplosion(asteroids::Coords pos) {
	for (unsigned int i = 0; i < 20; i++) {
		float x = 0.002f * cos(asteroids::randomF(M_PI));
		float y = 0.002f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		this->explosionBullets.insert(std::make_pair(bullet, new AsteroidsView(bullet, this->shader)));
	}
}

int AsteroidsController::DestroySpaceCraft(SpaceCraftModel* spaceCraft) {
	this->CreateExplosion(spaceCraft->pos);
	this->lifes.erase(std::prev(this->lifes.end()));
	this->sound.playSpaceCraftDestructionSound();
	spaceCraft->forward = { 0.0f, 0.0f };
	spaceCraft->pos = { 0.0f, 0.0f };
	spaceCraft->rotation = 0.0f;
	return -100;
}

unsigned int AsteroidsController::DestroySaucer(SaucerModel* saucer) {
	saucer->isActive = false;
	this->CreateExplosion(saucer->pos);
	this->sound.playSaucerDestructionSound();
	return saucer->score;
}

unsigned int AsteroidsController::SplitAsteroid(std::pair<AsteroidModel*, AsteroidsView*> asteroidPair) {
	this->asteroids.erase(asteroidPair);
	auto asteroid = asteroidPair.first;
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
		this->asteroids.insert(this->CreateAsteroid(score, asteroid->pos, asteroid->size * 0.4f, asteroid->killCount));
		this->asteroids.insert(this->CreateAsteroid(score, asteroid->pos, asteroid->size * 0.4f, asteroid->killCount));
	}
	return asteroid->score;
}


