#include "AsteroidsController.h"

AsteroidsController::AsteroidsController(GLFWwindow* w)
	:shader(Shader("res/shaders/basic.shader")),
	spaceShipModel(SpaceShipModel()),
	spaceShipView(AsteroidsView(&spaceShipModel, shader)),
	window(w)
{
	srand((unsigned int)time(NULL));
	for (auto i = 0; i < 3; i++) {
		auto life = new SpaceShipModel();
		life->size = 0.07f;			
		life->rotation = M_PI / 2;
		lifes.insert(std::make_pair(std::shared_ptr<SpaceShipModel>(life), std::shared_ptr<AsteroidsView>(new AsteroidsView(life, shader))));
	}

	this->saucer = this->CreateBigSaucer();
	this->saucer.first->isActive = false;
	this->sound.Initialize();

	for (unsigned int i = 0; i < 10; i++) {
		numbers[i] = CreateNumber(i);
	}
}

AsteroidsController::~AsteroidsController()
{
}

bool AsteroidsController::GameTick()
{	
	if (this->gameIsRunning) {
		this->sound.pitchBackgroundSound(0.0001f * this->soundPitch++ + 1.0f);

		this->gameIsRunning = this->CheckLifes(this->lifes);

		MoveSpaceShipAndUpdateInput();

		MoveOrCreateSaucer();

		MoveOrCreateAsteroids();

		MoveSpaceShipBullets();

		MoveSaucerBullets();

		MoveExplosionParticles();

		MoveSpaceShipExplosionParticles();

		DrawHighScore();
	}
	if (!this->gameIsRunning && !this->exitGame) {
		DrawEndScreen();
	}
	if (!this->gameIsRunning && this->exitGame) {
		return false;
	}
	return true;
}

void AsteroidsController::MoveSpaceShipAndUpdateInput()
{
	if (this->waitForspaceShip > WAIT_FOR_SPACESHIP) {
		this->sound.playBackgroundSound();
		this->spaceShipModel.isActive = true;
		UpdateInput(window, this->spaceShipModel);
		this->spaceShipModel.pos = UpdatePosition(this->spaceShipModel.forward, this->spaceShipModel.pos);

		if (this->spaceShipModel.isActive) {
			if (this->spaceShipModel.isBoosted && this->gameTick % 2 == 0) {
				this->spaceShipView.GameTick(14);
			}
			else {
				this->spaceShipView.GameTick(10);
			}
		}		
	}
	else {
		this->waitForspaceShip++;
	}
}

void AsteroidsController::MoveExplosionParticles()
{
	for (auto bulletIt = this->explosionBullets.begin(); bulletIt != this->explosionBullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->BulletTick();
		if (bullet.first->tickCount++ > 50) {
			this->explosionBullets.erase(bullet);
		}
	}		
}
	
void AsteroidsController::MoveSpaceShipExplosionParticles()
{
	for (auto bulletIt = this->spaceShipExplosionBullets.begin(); bulletIt != this->spaceShipExplosionBullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->GameTick();
		if (bullet.first->tickCount++ > 100) {
			this->spaceShipExplosionBullets.erase(bullet);
		}
	}		
}

void AsteroidsController::DrawHighScore()
{
	float count = 0.02f;
	std::vector<int> digits;
	collectDigits(digits, this->highscore);
	for (int i = 0; i < digits.size() ; i++) {
		numbers[digits[i]].first->pos = { -1.0f + count , 0.95f };
		numbers[digits[i]].second->GameTick();
		count += 0.03f;
	}
}


void AsteroidsController::DrawEndScreen()
{
	this->sound.stopAllSoundLoops();
	float count = 0.0f;
	std::vector<int> digits;
	collectDigits(digits, this->highscore);
	for (int i = (int)digits.size() - 1; i >= 0 ; i--) {
		numbers[digits[i]].first->pos = { 0.03f - count , -0.02f };
		numbers[digits[i]].second->GameTick();
		count += 0.03f;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		this->exitGame = true;
	}
}

void AsteroidsController::collectDigits(std::vector<int>& digits, unsigned int num) {
	if (num > 9) {
		collectDigits(digits, num / 10);
	}
	digits.push_back(num % 10);
}

void AsteroidsController::resetPosition(SpaceShipModel& spaceShipModel)
{
	spaceShipModel.isActive = false;
	this->waitForspaceShip = 200;		
	spaceShipModel.pos = { asteroids::randomF(1.0f), asteroids::randomF(1.0f) };
	spaceShipModel.forward = { 0.0f, 0.0f };
}
	

void AsteroidsController::MoveSaucerBullets()
{
	for (auto bulletIt = this->saucerBullets.begin(); bulletIt != this->saucerBullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->BulletTick();
		if (bullet.first->tickCount++ > BULLET_LIFETIME) {
			this->saucerBullets.erase(bullet);
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it++;
				if (IsCollision(bullet.first->pos, bullet.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
					this->saucerBullets.erase(bullet);
					this->highscore += this->SplitAsteroid(asteroid, bullet.first->forward, BULLET_IMPACT);
					break;
				}
			}
			if (spaceShipModel.isActive && IsCollision(spaceShipModel.pos, spaceShipModel.GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
				this->saucerBullets.erase(bullet);
				this->waitForspaceShip = this->DestroySpaceShip(this->spaceShipModel);
			}
		}
	}
}

void AsteroidsController::MoveSpaceShipBullets()
{
	for (auto bulletIt = this->bullets.begin(); bulletIt != this->bullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
		bullet.second->BulletTick();
		if (bullet.first->tickCount++ > BULLET_LIFETIME) {
			this->bullets.erase(bullet);
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it++;
				if (IsCollision(bullet.first->pos, bullet.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
					this->bullets.erase(bullet);
					this->highscore += this->SplitAsteroid(asteroid, bullet.first->forward, BULLET_IMPACT);
					break;
				}
			}
			if (this->saucer.first->isActive && IsCollision(saucer.first->pos, saucer.first->GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
				this->bullets.erase(bullet);
				this->highscore += this->DestroySaucer(this->saucer.first);

				std::cout << "current Score = " << this->highscore << std::endl;
			}
		}
	}
}

void AsteroidsController::MoveOrCreateAsteroids()
{
	if (this->asteroids.size() == 0 && !this->saucer.first->isActive) {
		this->waitedForAsteroids++;
	}
	if (this->asteroids.size() == 0 && !this->saucer.first->isActive && this->waitedForAsteroids > WAIT_FOR_ASTEROIDS) {
		this->soundPitch = 0.0f;
		for (auto i = 0; i < 4; i++) {
			this->asteroids.insert(this->CreateAsteroid(SCORE_LARGE_ASTEROID));
		}
		this->waitedForAsteroids = 0;
	}
	else {
		for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
		{
			auto asteroid = *it;
			it++;
			asteroid.first->pos = UpdatePosition(asteroid.first->forward, asteroid.first->pos);
			asteroid.second->GameTick();

			if (spaceShipModel.isActive && IsCollision(spaceShipModel.pos, spaceShipModel.GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				this->highscore += this->SplitAsteroid(asteroid, spaceShipModel.forward, BIG_IMPACT);
				this->waitForspaceShip = this->DestroySpaceShip(this->spaceShipModel);
			}
			if (this->saucer.first->isActive && IsCollision(saucer.first->pos, this->saucer.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
				this->highscore += this->SplitAsteroid(asteroid, saucer.first->forward, BIG_IMPACT);
				this->highscore += this->DestroySaucer(this->saucer.first);
			}
		}
	}
}

void AsteroidsController::MoveOrCreateSaucer()
{
	if (this->saucer.first->isActive) {
		this->saucer.first->pos = UpdatePosition(this->saucer.first->forward, this->saucer.first->pos);
		this->saucer.second->GameTick();

		this->saucer.first->ticks++;
		if (this->saucer.first->ticks % SAUCER_SHOOTING_INTERVALL == 0 && this->spaceShipModel.isActive) {
			if (this->saucer.first->isSmallSaucer) {
				this->saucerBullets.insert(CreateSmallSaucerBullet(this->saucer.first->pos, this->spaceShipModel.pos));
			}
			else {
				this->saucerBullets.insert(CreateSaucerBullet(this->saucer.first->pos));
			}
				
		}
		if (this->saucer.first->ticks % SAUCER_CHANGE_DIRECTION_INTERVALL == 0) {
			this->saucer.first->forward.y = asteroids::randomF(-0.005f);
		}
		if (abs(this->saucer.first->pos.x) == 1.0f) {
			this->saucer.first->isActive = false;	
			this->sound.stopSaucerSound();
		}
		if (spaceShipModel.isActive && IsCollision(this->saucer.first->pos, this->saucer.first->GetCollisionRadius(), spaceShipModel.pos, spaceShipModel.GetCollisionRadius())) {
			this->highscore += this->DestroySaucer(this->saucer.first);
			this->waitForspaceShip = this->DestroySpaceShip(this->spaceShipModel);
		}
	}

	else {
		this->gameTick++;
		if (this->gameTick % Wait_FOR_SMALL_SAUCER == 0) {
			this->saucer = this->CreateSmallSaucer();
			this->sound.playSmallSaucerSound();
		}
		else if (this->gameTick % Wait_FOR_SAUCER == 0) {
			this->saucer = this->CreateBigSaucer();
			this->sound.playSaucerSound();
		}
	}
}

bool AsteroidsController::CheckLifes(std::set<std::pair<std::shared_ptr<SpaceShipModel>, std::shared_ptr<AsteroidsView>>> lifes) {
	if (lifes.size() == 0) {
		return false;
	}
	float distance = 0.02f;
	for (auto lifeIt = lifes.begin(); lifeIt != lifes.end(); )
	{
		auto life = *lifeIt++;
		life.first->pos = { -1.0f + distance, 0.9f };
		life.second->GameTick(life.first->bufferSize / 2 - 4);
		distance += 0.05f;
	}
	return true;
}

void AsteroidsController::UpdateInput(GLFWwindow* window, SpaceShipModel& spaceShip)
{
	spaceShip.forward.x *= 0.997f;
	spaceShip.forward.y *= 0.997f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		float x = 0.0001f * cos(spaceShip.rotation);
		float y = 0.0001f * sin(spaceShip.rotation);
		spaceShip.forward.x += x;
		spaceShip.forward.y += y;
		spaceShip.isBoosted = true;
		this->sound.playThrustSound();
	}
	else {
		spaceShip.isBoosted = false;
		this->sound.stopThrustSound();
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		spaceShip.rotation -= 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		spaceShip.rotation += 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		this->resetPosition(spaceShip);
	}
		
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
	{
		this->shooted = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if (!this->shooted && this->bullets.size() < 4) {
			this->bullets.insert(CreateBullet(spaceShipModel.pos, spaceShip.rotation));
			this->shooted = true;
			this->sound.playShootingSound();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		this->escapeKeyDown = true;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{
		if (this->escapeKeyDown) {
			this->gameIsRunning = false;
			this->escapeKeyDown = false;
		}
	}
}

void AsteroidsController::CreateExplosion(asteroids::Coords pos) {
	for (unsigned int i = 0; i < 20; i++) {
		float x = 0.002f * cos(asteroids::randomF(M_PI));
		float y = 0.002f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		this->explosionBullets.insert(std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet, this->shader))));
	}
}
	
void AsteroidsController::CreatespaceShipExplosion(asteroids::Coords pos) {
	for (unsigned int i = 0; i < 10; i++) {
		float x = 0.001f * cos(asteroids::randomF(M_PI));
		float y = 0.001f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		bullet->bufferSize = 4;
		bullet->rotation = abs(asteroids::randomF(M_PI));
		this->spaceShipExplosionBullets.insert(std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet, this->shader))));
	}
}

int AsteroidsController::DestroySpaceShip(SpaceShipModel& spaceShip) {
	this->CreatespaceShipExplosion(spaceShip.pos);
	this->lifes.erase(std::prev(this->lifes.end()));
	this->sound.playspaceShipDestructionSound();
	this->sound.stopThrustSound();
	this->sound.stopBackgroundSound();

	spaceShip.forward = { 0.0f, 0.0f };
	spaceShip.pos = { 0.0f, 0.0f };
	spaceShip.rotation = 0.0f;
	spaceShip.isActive = false;
	spaceShip.isBoosted = false;		
	return 0;
}

unsigned int AsteroidsController::DestroySaucer(std::shared_ptr<SaucerModel> saucer) {
	saucer->isActive = false;
	this->CreateExplosion(saucer->pos);
	this->sound.playSaucerDestructionSound();
	this->sound.stopSaucerSound();
	return saucer->score;
}

unsigned int AsteroidsController::SplitAsteroid(std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>> asteroidPair, asteroids::Coords forward, float impact) {
	auto asteroid = *asteroidPair.first;
	this->asteroids.erase(asteroidPair);
	this->sound.playAsteroidDestructionSound((float) (asteroid.killCount+ 1));
	this->CreateExplosion(asteroid.pos);
	if (asteroid.killCount < 2) {
		asteroid.killCount++;
		int score = SCORE_LARGE_ASTEROID;
		if (asteroid.killCount == 1) {
			score = SCORE_MEDIUM_ASTEROID;
		}
		if (asteroid.killCount == 2) {
			score = SCORE_SMALL_ASTEROID;
		}
		float rotation = abs(asteroids::randomF(M_PI / 4));
		float x = (forward.x * impact + asteroid.forward.x) * cos(rotation);
		float y = (forward.y * impact + asteroid.forward.y) * sin(rotation);
		this->asteroids.insert(this->CreateAsteroid(score, asteroid.pos, asteroid.size * 0.45f, asteroid.killCount, { x, y }));
		rotation = asteroids::randomF(M_PI / 4);
		x = (forward.x * impact + asteroid.forward.x) * cos(rotation);
		y = (forward.y * impact + asteroid.forward.y) * sin(rotation);
		this->asteroids.insert(this->CreateAsteroid(score, asteroid.pos, asteroid.size * 0.45f, asteroid.killCount, { x, y }));
	}
	return asteroid.score;
}

std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateAsteroid(int score, asteroids::Coords pos, float size, unsigned int killCount, asteroids::Coords forward) {
	auto asteroid = new AsteroidModel(pos, size, killCount, score, forward);
	return std::make_pair(std::shared_ptr<AsteroidModel>(asteroid), std::shared_ptr <AsteroidsView>(new AsteroidsView(asteroid, this->shader)));
}
std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateBigSaucer() {
	auto saucer = new SaucerModel(SCORE_LARGE_SAUCER);
	return std::make_pair(std::shared_ptr<SaucerModel>(saucer), std::shared_ptr<AsteroidsView>(new AsteroidsView(saucer, this->shader)));
}
std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateSmallSaucer() {
	auto saucer = new SaucerModel(SCORE_SMALL_SAUCER, 0.006f, 0.004f);
	saucer->isSmallSaucer = true;
	return std::make_pair(std::shared_ptr<SaucerModel>(saucer), std::shared_ptr<AsteroidsView>(new AsteroidsView(saucer, this->shader)));
}

std::pair< std::shared_ptr<NumberModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateNumber(int number)
{
	auto numberModel = new NumberModel(number);
	return std::make_pair(std::shared_ptr<NumberModel>(numberModel), std::shared_ptr<AsteroidsView>(new AsteroidsView(numberModel, this->shader)));
}

std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateBullet(asteroids::Coords pos, float rotation) {
	float x = 0.02f * cos(rotation);
	float y = 0.02f * sin(rotation);
	auto bullet = new BulletModel(pos, { x, y });
	return std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet, this->shader)));
}

std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateSaucerBullet(asteroids::Coords pos) {
	float rotation = asteroids::randomF(M_PI);
	float x = 0.01f * cos(rotation) - 0.01f * sin(rotation);
	float y = 0.01f * sin(rotation) + 0.01f * cos(rotation);
	auto bullet = new BulletModel(pos, { x, y });
	return std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet, this->shader)));
}

std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateSmallSaucerBullet(asteroids::Coords pos, asteroids::Coords target) {
	float x = target.x - pos.x;
	float y = target.y - pos.y;
	asteroids::Coords forward = asteroids::normalize(x, y);
	auto bullet = new BulletModel(pos, forward);
	return std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet, this->shader)));
}
