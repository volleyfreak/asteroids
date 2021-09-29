#include "AsteroidsController.h"

AsteroidsController::AsteroidsController(GLFWwindow* w)
	:shader(Shader("res/shaders/basic.shader")),
	spaceShip(SpaceShipModel()),
	spaceShipView(AsteroidsView(&spaceShip)),
	window(w)
{
	srand((unsigned int)time(NULL));
	for (auto i = 0; i < 3; i++) {
		auto life = new SpaceShipModel();
		life->size = 0.07f;			
		life->rotation = M_PI / 2;
		lifes.insert(std::make_pair(std::shared_ptr<SpaceShipModel>(life), std::shared_ptr<AsteroidsView>(new AsteroidsView(life))));
	}

	this->saucer = this->CreateBigSaucerReferences();
	this->saucer.first->isActive = false;
	this->sound.Initialize();

	for (unsigned int i = 0; i < 10; i++) {
		numbers[i] = CreateNumberReferences(i);
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
		this->spaceShip.isActive = true;
		UpdateInput(window, this->spaceShip);
		this->spaceShip.Move();

		if (this->spaceShip.isActive) {
			if (this->spaceShip.isBoosted && this->gameTick % 2 == 0) {
				this->spaceShipView.Draw(shader, 14);
			}
			else {
				this->spaceShipView.Draw(shader, 10);
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
		bullet.first.get()->Move();
		bullet.second->DrawBullet(shader);
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
		bullet.first->Move();
		bullet.second->Draw(shader);
		if (bullet.first->tickCount++ > 100) {
			this->spaceShipExplosionBullets.erase(bullet);
		}
	}		
}

void AsteroidsController::DrawHighScore()
{
	float count = 0.02f;
	std::vector<int> digits;
	getDigitsInReverseOrder(digits, this->highscore);
	for (int i = 0; i < digits.size() ; i++) {
		numbers[digits[i]].first->position = { -1.0f + count , 0.95f };
		numbers[digits[i]].second->Draw(shader);
		count += 0.03f;
	}
}

void AsteroidsController::DrawEndScreen()
{
	this->sound.stopAllSoundLoops();
	float count = 0.0f;
	std::vector<int> digits;
	getDigitsInReverseOrder(digits, this->highscore);
	for (int i = (int)digits.size() - 1; i >= 0 ; i--) {
		numbers[digits[i]].first->position = { 0.03f - count , -0.02f };
		numbers[digits[i]].second->Draw(shader);
		count += 0.03f;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		this->exitGame = true;
	}
}

void AsteroidsController::getDigitsInReverseOrder(std::vector<int>& digits, unsigned int num) {
	if (num > 9) {
		getDigitsInReverseOrder(digits, num / 10);
	}
	digits.push_back(num % 10);
}

void AsteroidsController::resetPosition(SpaceShipModel& spaceShipModel)
{
	spaceShipModel.isActive = false;
	this->waitForspaceShip = 200;		
	spaceShipModel.position = { asteroids::randomF(1.0f), asteroids::randomF(1.0f) };
	spaceShipModel.forward = { 0.0f, 0.0f };
}	

void AsteroidsController::MoveSaucerBullets()
{
	for (auto bulletIt = this->saucer.first->bullets.begin(); bulletIt != this->saucer.first->bullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet->Move();
		auto view = AsteroidsView(bullet.get());
		view.DrawBullet(shader);
		if (bullet->tickCount++ > BULLET_LIFETIME) {
			this->saucer.first->bullets.erase(bullet);
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it++;
				if (bullet->IsCollision(*asteroid.first)) {
					this->saucer.first->bullets.erase(bullet);
					this->highscore += this->SplitAsteroid(asteroid, bullet->forward, BULLET_IMPACT);
					break;
				}
			}
			if (spaceShip.isActive && bullet->IsCollision(this->spaceShip)) {
				this->saucer.first->bullets.erase(bullet);
				this->waitForspaceShip = this->DestroySpaceShip(this->spaceShip);
			}
		}
	}
}

void AsteroidsController::MoveSpaceShipBullets()
{

	for (auto bulletIt = this->spaceShip.bullets.begin(); bulletIt != this->spaceShip.bullets.end(); )
	{
		auto bullet = *bulletIt++;
		bullet->Move();
		auto view = AsteroidsView(bullet.get());
		view.DrawBullet(shader);
		if (bullet->tickCount++ > BULLET_LIFETIME) {
			this->spaceShip.bullets.erase(bullet);
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it++;
				if (bullet->IsCollision(*asteroid.first)){
					this->spaceShip.bullets.erase(bullet);
					this->highscore += this->SplitAsteroid(asteroid, bullet->forward, BULLET_IMPACT);
					break;
				}
			}
			if (this->saucer.first->isActive && bullet->IsCollision(*saucer.first)) {
				this->spaceShip.bullets.erase(bullet);
				this->highscore += this->DestroySaucer(this->saucer.first);
			}
		}		
	}
}

void AsteroidsController::MoveOrCreateAsteroids()
{
	if (this->asteroids.size() == 0 && !this->saucer.first->isActive) {
		this->waitForAsteroids++;
	}
	if (this->asteroids.size() == 0 && !this->saucer.first->isActive && this->waitForAsteroids > WAIT_FOR_ASTEROIDS) {
		this->soundPitch = 0.0f;
		for (auto i = 0; i < 4; i++) {
			this->asteroids.insert(this->CreateAsteroidReferences(SCORE_LARGE_ASTEROID));
		}
		this->waitForAsteroids = 0;
	}
	else {
		for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
		{
			auto asteroid = *it;
			it++;
			asteroid.first->Move();
			asteroid.second->Draw(shader);

			if (spaceShip.isActive && asteroid.first->IsCollision(this->spaceShip)) {
				this->highscore += this->SplitAsteroid(asteroid, spaceShip.forward, BIG_IMPACT);
				this->waitForspaceShip = this->DestroySpaceShip(this->spaceShip);
			}
			if (this->saucer.first->isActive && asteroid.first->IsCollision(*saucer.first)) {
				this->highscore += this->SplitAsteroid(asteroid, saucer.first->forward, BIG_IMPACT);
				this->highscore += this->DestroySaucer(this->saucer.first);
			}
		}
	}
}

void AsteroidsController::MoveOrCreateSaucer()
{
	if (this->saucer.first->isActive) {
		this->saucer.first->Move();
		this->saucer.second->Draw(shader);

		this->saucer.first->ticks++;
		if (this->saucer.first->ticks % SAUCER_SHOOTING_INTERVALL == 0 && this->spaceShip.isActive) {
			if (this->saucer.first->isSmallSaucer) {
				this->saucer.first->ShootTargetedBullet(this->spaceShip.position);
			}
			else {
				this->saucer.first->ShootRandomBullet();
			}
				
		}
		if (this->saucer.first->ticks % SAUCER_CHANGE_DIRECTION_INTERVALL == 0) {
			this->saucer.first->forward.y = asteroids::randomF(-0.005f);
		}
		if (abs(this->saucer.first->position.x) == 1.0f) {
			this->saucer.first->isActive = false;	
			this->sound.stopSaucerSound();
		}
		if (spaceShip.isActive && saucer.first->IsCollision(spaceShip)) {
			this->highscore += this->DestroySaucer(this->saucer.first);
			this->waitForspaceShip = this->DestroySpaceShip(this->spaceShip);
		}
	}

	else {
		this->gameTick++;
		if (this->gameTick % Wait_FOR_SMALL_SAUCER == 0) {
			this->saucer = this->CreateSmallSaucerReferences();
			this->sound.playSmallSaucerSound();
		}
		else if (this->gameTick % Wait_FOR_SAUCER == 0) {
			this->saucer = this->CreateBigSaucerReferences();
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
		life.first->position = { -1.0f + distance, 0.9f };
		life.second->Draw(shader, life.first->bufferSize / 2 - 4);
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
		this->spaceShip.shooted = false;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		if (this->spaceShip.shooted == false) {
			this->sound.playShootingSound();
		}
		this->spaceShip.shoot();		
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
		this->explosionBullets.insert(std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet))));
	}
}
	
void AsteroidsController::CreatespaceShipExplosion(asteroids::Coords pos) {
	for (unsigned int i = 0; i < 10; i++) {
		float x = 0.001f * cos(asteroids::randomF(M_PI));
		float y = 0.001f * sin(asteroids::randomF(M_PI));
		auto bullet = new BulletModel(pos, { x, y });
		bullet->bufferSize = 4;
		bullet->rotation = abs(asteroids::randomF(M_PI));
		this->spaceShipExplosionBullets.insert(std::make_pair(std::shared_ptr<BulletModel>(bullet), std::shared_ptr<AsteroidsView>(new AsteroidsView(bullet))));
	}
}

int AsteroidsController::DestroySpaceShip(SpaceShipModel& spaceShip) {
	this->CreatespaceShipExplosion(spaceShip.position);
	this->lifes.erase(std::prev(this->lifes.end()));
	this->sound.playspaceShipDestructionSound();
	this->sound.stopThrustSound();
	this->sound.stopBackgroundSound();

	spaceShip.forward = { 0.0f, 0.0f };
	spaceShip.position = { 0.0f, 0.0f };
	spaceShip.rotation = 0.0f;
	spaceShip.isActive = false;
	spaceShip.isBoosted = false;		
	return 0;
}

unsigned int AsteroidsController::DestroySaucer(std::shared_ptr<SaucerModel> saucer) {
	saucer->isActive = false;
	this->CreateExplosion(saucer->position);
	this->sound.playSaucerDestructionSound();
	this->sound.stopSaucerSound();
	return saucer->score;
}

unsigned int AsteroidsController::SplitAsteroid(std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>> asteroidPair, asteroids::Coords forward, float impact) {
	auto asteroid = *asteroidPair.first;
	this->asteroids.erase(asteroidPair);
	this->sound.playAsteroidDestructionSound((float) (asteroid.killCount+ 1));
	this->CreateExplosion(asteroid.position);
	if (asteroid.killCount < 2) {
		asteroid.killCount++;
		int score = SCORE_LARGE_ASTEROID;
		if (asteroid.killCount == 1) {
			score = SCORE_MEDIUM_ASTEROID;
		}
		if (asteroid.killCount == 2) {
			score = SCORE_SMALL_ASTEROID;
		}		
		this->asteroids.insert(this->CreateAsteroidReferences(score, asteroid.position, asteroid.size * 0.45f, asteroid.killCount, asteroid.CalculateImpact(impact)));
		this->asteroids.insert(this->CreateAsteroidReferences(score, asteroid.position, asteroid.size * 0.45f, asteroid.killCount, asteroid.CalculateImpact(impact)));
	}
	return asteroid.score;
}

std::pair<std::shared_ptr<AsteroidModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateAsteroidReferences(int score, asteroids::Coords pos, float size, unsigned int killCount, asteroids::Coords forward) {
	auto asteroid = new AsteroidModel(pos, size, killCount, score, forward);
	return std::make_pair(std::shared_ptr<AsteroidModel>(asteroid), std::shared_ptr <AsteroidsView>(new AsteroidsView(asteroid)));
}
std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateBigSaucerReferences() {
	auto saucer = new SaucerModel(SCORE_LARGE_SAUCER);
	return std::make_pair(std::shared_ptr<SaucerModel>(saucer), std::shared_ptr<AsteroidsView>(new AsteroidsView(saucer)));
}
std::pair<std::shared_ptr<SaucerModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateSmallSaucerReferences() {
	auto saucer = new SaucerModel(SCORE_SMALL_SAUCER, 0.006f, 0.004f);
	saucer->isSmallSaucer = true;
	return std::make_pair(std::shared_ptr<SaucerModel>(saucer), std::shared_ptr<AsteroidsView>(new AsteroidsView(saucer)));
}

std::pair< std::shared_ptr<NumberModel>, std::shared_ptr<AsteroidsView>> AsteroidsController::CreateNumberReferences(int number)
{
	auto numberModel = new NumberModel(number);
	return std::make_pair(std::shared_ptr<NumberModel>(numberModel), std::shared_ptr<AsteroidsView>(new AsteroidsView(numberModel)));
}