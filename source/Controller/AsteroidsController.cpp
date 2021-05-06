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

bool CheckLifes(std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes) {
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

bool AsteroidsController::GameTick()
{
	this->sound.playBackgroundSound();
	//draw lifes
	if (CheckLifes(this->lifes)) {
		return true;
	}

	if (this->waitForSpaceCraft > 0) {
		UpdateInput(window, spaceCraftModel);
		UpdatePosition(&spaceCraftModel.forward, &spaceCraftModel.pos);

		if (this->spaceCraftModel.isBoosted && this->gameTick % 2 == 0) {
			this->spaceCraftView.GameTick(spaceCraftModel, spaceCraftModel.pos, 14);
		}
		else {
			this->spaceCraftView.GameTick(spaceCraftModel, spaceCraftModel.pos, 10);
		}
	}
	else {
		this->waitForSpaceCraft++;
	}

	// check if saucer exists
	if (this->saucer.first->isActive) { 
		UpdatePosition(&this->saucer.first->forward, &this->saucer.first->pos);
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
			this->destroySaucer(this->saucer.first);
			this->destroySpaceCraft();
		  }
	}
	//create saucer
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
	//draw bullets
	for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
	{		
		auto bullet = *bulletIt++;
		auto pos = UpdatePosition(&bullet.first->forward, &bullet.first->pos);
		bullet.second->BulletTick(bullet.first, pos, 1);
		if (bullet.first->tickCount++ > 100) {
			bullets.erase(bullet);
		}		
	}

	for (auto bulletIt = explosionBullets.begin(); bulletIt != explosionBullets.end(); )
	{
		auto bullet = *bulletIt++;
		auto pos = UpdatePosition(&bullet.first->forward, &bullet.first->pos);
		bullet.second->BulletTick(bullet.first, pos, 1);
		if (bullet.first->tickCount++ > 50) {
			explosionBullets.erase(bullet);
		}
	}

	for (auto bulletIt = saucerBullets.begin(); bulletIt != saucerBullets.end(); )
	{		
		auto bullet = *bulletIt++;
		auto pos = UpdatePosition(&bullet.first->forward, &bullet.first->pos);
		bullet.second->BulletTick(bullet.first, pos, 1);
		if (bullet.first->tickCount++ > 100) {
			saucerBullets.erase(bullet);
		}		
	}
	if (asteroids.size() == 0) {
		for (auto i = 0; i < 3; i++) {
			this->asteroids.insert(CreateAsteroid(SCORE_LARGE_ASTEROID));
		}
	}
	else {
		for (auto it = asteroids.begin(); it != asteroids.end();)
		{
			auto asteroid = *it++;
			auto pos = UpdatePosition(&asteroid.first->forward, &asteroid.first->pos);
			asteroid.second->GameTick(*asteroid.first, *pos, asteroid.first->bufferSize / 2);

			if (IsCollision(spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				SplitAsteroid(asteroid.first);
				asteroids.erase(asteroid);
				this->destroySpaceCraft();
			}
			if (this->saucer.first->isActive && IsCollision(saucer.first->pos, this->saucer.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				SplitAsteroid(asteroid.first);
				asteroids.erase(asteroid);
				this->destroySaucer(this->saucer.first);
			}
		}
	}	

	for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
	{
		auto bullet = *bulletIt++;
		for (auto it = asteroids.begin(); it != asteroids.end();)
		{
			auto asteroid = *it++;
			if (IsCollision(bullet.first->pos, bullet.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				bullets.erase(bullet);
				SplitAsteroid(asteroid.first);
				asteroids.erase(asteroid);
			}
		}
		if (this->saucer.first->isActive && IsCollision(saucer.first->pos, saucer.first->GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
			bullets.erase(bullet);
			this->destroySaucer(this->saucer.first);
		}
	}
	
	for (auto bulletIt = saucerBullets.begin(); bulletIt != saucerBullets.end(); )
	{
		auto bullet = *bulletIt++;
		for (auto it = asteroids.begin(); it != asteroids.end();)
		{
			auto asteroid = *it++;
			if (IsCollision(bullet.first->pos, bullet.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				saucerBullets.erase(bullet);
				SplitAsteroid(asteroid.first);
				asteroids.erase(asteroid);
			}
		}
		if (IsCollision(spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
			saucerBullets.erase(bullet);
			this->destroySpaceCraft();
		}
	}	
	return true;
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
