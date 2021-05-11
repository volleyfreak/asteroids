#include "AsteroidsController.h"


namespace asteroids {
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

		this->saucer = this->CreateBigSaucer();
		this->saucer.first->isActive = false;
		this->sound.Initialize();
		this->sound.playBackgroundSound();
	}

	AsteroidsController::~AsteroidsController()
	{
	}

	bool AsteroidsController::GameTick()
	{	
		this->sound.pitchBackgroundSound(0.0001f * this->soundPitch++ + 1.0);

		if (this->gameIsRunning) {
			this->gameIsRunning = this->CheckLifes(this->lifes);

			MoveSpaceCraft();

			MoveOrCreateSaucer();

			MoveOrCreateAsteroids();

			MoveSpaceCraftBullets();

			MoveSaucerBullets();

			MoveExplosionParticles();

			DrawHighScore();
		}
		else {
			DrawEndScreen();
		}
		return true;
	}

	void AsteroidsController::MoveSpaceCraft()
	{
		if (this->waitForSpaceCraft > 350) {
			this->spaceCraftModel.isActive = true;
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
	}

	void AsteroidsController::MoveExplosionParticles()
	{
		for (auto bulletIt = this->explosionBullets.begin(); bulletIt != this->explosionBullets.end(); )
		{
			auto bullet = *bulletIt++;
			bullet.first->pos = UpdatePosition(bullet.first->forward, bullet.first->pos);
			bullet.second->BulletTick(*bullet.first, bullet.first->pos, 1);
			if (bullet.first->tickCount++ > 50) {
				this->explosionBullets.erase(bullet);
			}
		}		
	}

	void AsteroidsController::DrawHighScore()
	{
		float count = 0.0f;
		std::vector<int> digits;
		collectDigits(digits, this->highscore);
		for (int i = digits.size() - 1; i >= 0 ; i--) {
			auto number = NumberModels(digits[i], { 0.95f - count , 0.95f});
			auto view = AsteroidsView(&number, this->shader);
			view.GameTick(number, number.pos, 15);
			count += 0.05f;
		}
	}

	void AsteroidsController::DrawEndScreen()
	{
		float count = 0.0f;
		std::vector<int> digits;
		collectDigits(digits, this->highscore);
		for (int i = digits.size() - 1; i >= 0 ; i--) {
			auto number = NumberModels(digits[i], { 0.05f - count , -0.05f});
			auto view = AsteroidsView(&number, this->shader);
			view.GameTick(number, number.pos, 15);
			count += 0.05f;
		}
	}

	// https://stackoverflow.com/questions/4615046/c-get-each-digit-in-int
	//todo move to void structs
	void AsteroidsController::collectDigits(std::vector<int>& digits, unsigned int num) {
		if (num > 9) {
			collectDigits(digits, num / 10);
		}
		digits.push_back(num % 10);
	}
	

	void AsteroidsController::MoveSaucerBullets()
	{
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
						this->highscore += this->SplitAsteroid(asteroid, bullet.first->forward, 0.2f);

					}
				}
				if (spaceCraftModel.isActive && IsCollision(spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius(), bullet.first->pos, bullet.first->GetCollisionRadius())) {
					this->saucerBullets.erase(bullet);
					this->waitForSpaceCraft = this->DestroySpaceCraft(this->spaceCraftModel);
				}
			}
		}
	}

	void AsteroidsController::MoveSpaceCraftBullets()
	{
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
						this->highscore += this->SplitAsteroid(asteroid, bullet.first->forward, 0.2f);
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
		if (this->asteroids.size() == 0) {
			for (auto i = 0; i < 4; i++) {
				this->asteroids.insert(this->CreateAsteroid(SCORE_LARGE_ASTEROID));
			}
		}
		else {
			for (auto it = this->asteroids.begin(); it != this->asteroids.end();)
			{
				auto asteroid = *it;
				it++;
				asteroid.first->pos = UpdatePosition(asteroid.first->forward, asteroid.first->pos);
				asteroid.second->GameTick(*asteroid.first, asteroid.first->pos, asteroid.first->bufferSize / 2);

				if (spaceCraftModel.isActive && IsCollision(spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
					this->highscore += this->SplitAsteroid(asteroid, spaceCraftModel.forward, 1.0f);
					this->waitForSpaceCraft = this->DestroySpaceCraft(this->spaceCraftModel);
				}
				if (this->saucer.first->isActive && IsCollision(saucer.first->pos, this->saucer.first->GetCollisionRadius(), asteroid.first->pos, asteroid.first->GetCollisionRadius())) {
					this->highscore += this->SplitAsteroid(asteroid, saucer.first->forward, 1.0f);
					this->highscore += this->DestroySaucer(this->saucer.first);
				}
			}
		}
	}

	void AsteroidsController::MoveOrCreateSaucer()
	{
		if (this->saucer.first->isActive) {
			this->saucer.first->pos = UpdatePosition(this->saucer.first->forward, this->saucer.first->pos);
			this->saucer.second->GameTick(*this->saucer.first, this->saucer.first->pos, 20);

			this->saucer.first->ticks++;
			if (this->saucer.first->ticks % 100 == 0 && this->spaceCraftModel.isActive) {
				if (this->saucer.first->isSmallSaucer) {
					this->saucerBullets.insert(CreateSmallSaucerBullet(this->saucer.first->pos, this->spaceCraftModel.pos));
				}
				else {
					this->saucerBullets.insert(CreateSaucerBullet(this->saucer.first->pos));
				}
				
			}
			if (this->saucer.first->ticks % 133 == 0) {
				this->saucer.first->forward.y = asteroids::randomF(-0.005f);
			}
			if (abs(this->saucer.first->pos.x) == 1.0f) {
				this->saucer.first->isActive = false;	
				this->sound.stopSaucerSound();
			}
			if (spaceCraftModel.isActive && IsCollision(this->saucer.first->pos, this->saucer.first->GetCollisionRadius(), spaceCraftModel.pos, spaceCraftModel.GetCollisionRadius())) {
				this->highscore += this->DestroySaucer(this->saucer.first);
				this->waitForSpaceCraft = this->DestroySpaceCraft(this->spaceCraftModel);
			}
		}

		else {
			this->gameTick++;
			if (this->gameTick % 300 == 0) {
				this->saucer = this->CreateSmallSaucer();
				this->sound.playSmallSaucerSound();
			}
			else if (this->gameTick % 100 == 0) {
				this->saucer = this->CreateBigSaucer();
				this->sound.playSaucerSound();
			}
		}
	}

	bool AsteroidsController::CheckLifes(std::set<std::pair<SpaceCraftModel*, AsteroidsView*>> lifes) {
		if (lifes.size() == 0) {
			return false;
		}
		float xPos = 0;
		for (auto lifeIt = lifes.begin(); lifeIt != lifes.end(); )
		{
			auto life = *lifeIt++;
			life.second->GameTick(*life.first, { -0.95f + xPos, 0.95f }, life.first->bufferSize / 2 - 4);
			xPos += 0.07f;
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
			this->sound.playThrustSound();
		}
		else {
			spaceCraft.isBoosted = false;
			this->sound.stopThrustSound();
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
				this->sound.playShootingSound();
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

	int AsteroidsController::DestroySpaceCraft(SpaceCraftModel& spaceCraft) {
		this->CreateExplosion(spaceCraft.pos);
		this->lifes.erase(std::prev(this->lifes.end()));
		this->sound.playSpaceCraftDestructionSound();
		this->sound.stopThrustSound();
		spaceCraft.forward = { 0.0f, 0.0f };
		spaceCraft.pos = { 0.0f, 0.0f };
		spaceCraft.rotation = 0.0f;
		spaceCraft.isActive = false;
		spaceCraft.isBoosted = false;		
		return 0;
	}

	unsigned int AsteroidsController::DestroySaucer(SaucerModel* saucer) {
		saucer->isActive = false;
		this->CreateExplosion(saucer->pos);
		this->sound.playSaucerDestructionSound();
		this->sound.stopSaucerSound();
		return saucer->score;
	}

	unsigned int AsteroidsController::SplitAsteroid(std::pair<AsteroidModel*, AsteroidsView*> asteroidPair, asteroids::Coords forward, float impact) {
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
			float rotation = asteroids::randomF(M_PI / 4);
			float x = forward.x * impact + asteroid->forward.x * cos(rotation);
			float y = forward.y * impact + asteroid->forward.y * sin(rotation);
			this->asteroids.insert(this->CreateAsteroid(score, asteroid->pos, asteroid->size * 0.4f, asteroid->killCount, { x, y }));
			rotation = asteroids::randomF(M_PI / 4);
			x = forward.x * impact + asteroid->forward.x * cos(rotation);
			y = forward.y * impact + asteroid->forward.y * sin(rotation);
			this->asteroids.insert(this->CreateAsteroid(score, asteroid->pos, asteroid->size * 0.4f, asteroid->killCount, { x, y }));
		}
		return asteroid->score;
	}

	std::pair<AsteroidModel*, AsteroidsView*> AsteroidsController::CreateAsteroid(int score, asteroids::Coords pos, float size, unsigned int killCount, asteroids::Coords forward) {
		auto asteroid = new AsteroidModel(pos, size, killCount, score, forward);
		return std::make_pair(asteroid, new AsteroidsView(asteroid, this->shader));
	}
	std::pair<SaucerModel*, AsteroidsView*> AsteroidsController::CreateBigSaucer() {
		auto saucer = new SaucerModel(SCORE_LARGE_SAUCER);
		return std::make_pair(saucer, new AsteroidsView(saucer, this->shader));
	}
	std::pair<SaucerModel*, AsteroidsView*> AsteroidsController::CreateSmallSaucer() {
		auto saucer = new SaucerModel(SCORE_SMALL_SAUCER, 0.006f, 0.004f);
		saucer->isSmallSaucer = true;
		return std::make_pair(saucer, new AsteroidsView(saucer, this->shader));
	}

	std::pair<BulletModel*, AsteroidsView*> AsteroidsController::CreateBullet(asteroids::Coords pos, float rotation) {
		float x = 0.02f * cos(rotation);
		float y = 0.02f * sin(rotation);
		auto bullet = new BulletModel(pos, { x, y });
		return std::make_pair(bullet, new AsteroidsView(bullet, this->shader));
	}

	std::pair<BulletModel*, AsteroidsView*> AsteroidsController::CreateSaucerBullet(asteroids::Coords pos) {
		float rotation = asteroids::randomF(M_PI);
		float x = 0.01f * cos(rotation) - 0.01f * sin(rotation);
		float y = 0.01f * sin(rotation) + 0.01f * cos(rotation);
		auto bullet = new BulletModel(pos, { x, y });
		return std::make_pair(bullet, new AsteroidsView(bullet, this->shader));
	}

	std::pair<BulletModel*, AsteroidsView*> AsteroidsController::CreateSmallSaucerBullet(asteroids::Coords pos, asteroids::Coords target) {
		float x = target.x - pos.x;
		float y = target.y - pos.y;
		asteroids::Coords forward = normalize(x, y);
		auto bullet = new BulletModel(pos, forward);
		return std::make_pair(bullet, new AsteroidsView(bullet, this->shader));
	}


}