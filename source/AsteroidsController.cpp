#include "AsteroidsController.h"



AsteroidsController::AsteroidsController(GLFWwindow* w)
	:shader(Shader("res/shaders/basic.shader")), 
	spaceCraftModel(SpaceCraftModel()),
	spaceCraftView(AsteroidsView(&spaceCraftModel, shader)),
	window(w)
{
	srand(time(NULL));
	for (auto i = 0; i < 4; i++) {
		CreateAsteroid();
	}
	auto saucer = new BigSaucerModel();
	this->saucer = std::make_pair(saucer, new AsteroidsView(saucer, shader));
}

AsteroidsController::~AsteroidsController()
{
}

bool AsteroidsController::GameTick()
{
	UpdateInput(window, spaceCraftModel.forward, spaceCraftModel.rotation);
	UpdatePosition(&spaceCraftModel.forward, &spaceCraftModel.pos);
	this->spaceCraftView.GameTick(spaceCraftModel, spaceCraftModel.pos, 10);

	if (this->saucer.first->isActive) {
		UpdatePosition(&this->saucer.first->forward, &this->saucer.first->pos);
		this->saucer.second->GameTick(*this->saucer.first, this->saucer.first->pos, 20);
		if (this->saucer.first->ticks++ % 100 == 0) {
			CreateBullet(this->saucer.first->pos, this->saucer.first->rotation);
		}
	}
	

	for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
	{		
		auto bullet = *bulletIt++;
		auto pos = UpdatePosition(&bullet.first->forward, &bullet.first->pos);
		bullet.second->BulletTick(bullet.first, pos, 1);
		if (bullet.first->tickCount++ > 180) {
			bullets.erase(bullet);
		}		
	}
	for (auto it = asteroids.begin(); it != asteroids.end();)
	{		
		auto asteroid = *it++;
		auto pos = UpdatePosition(&asteroid.first->forward, &asteroid.first->pos);
		asteroid.second->GameTick(*asteroid.first, *pos, 20);		
		
		if (IsCollision(spaceCraftModel.pos, 0.03f, asteroid.first->pos, asteroid.first->size * 3)) {
			SplitAsteroid(asteroid.first);
			asteroids.erase(asteroid);
			//gameIsRunning = false;
		}
		if (IsCollision(saucer.first->pos, 0.03f, asteroid.first->pos, asteroid.first->size * 3)) {
			SplitAsteroid(asteroid.first);
			asteroids.erase(asteroid);
			//gameIsRunning = false;
		}
	}
	for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); )
	{
		auto bullet = *bulletIt++;
		for (auto it = asteroids.begin(); it != asteroids.end();)
		{
			auto asteroid = *it++;
			if (IsCollision(bullet.first->pos, 0.01f, asteroid.first->pos, asteroid.first->size * 3)) {
				bullets.erase(bullet);
				SplitAsteroid(asteroid.first);
				asteroids.erase(asteroid);
			}
		}
		if (IsCollision(saucer.first->pos, saucer.first->size * 2, bullet.first->pos, bullet.first->size * 3)) {
			//todo: destroy saucer
			//gameIsRunning = false;
		}
	}
	return true;
}

void AsteroidsController::TransferCoordinates(float& x, float& y)
{
	if (x > 1)
		x = -1;
	if (x < -1)
		x = 1;
	if (y > 1)
		y = -1;
	if (y < -1)
		y = 1;
}


