#pragma once
#include "PhysicEngine.h"
#include "Utils.h"
#include <memory>
#include <set>

#include "Bullet/BulletModel.h"
#include "View/AsteroidsView.h"
#include <Sound/SoundController.h>


//Main playing figure, a spaceship that is moved by player input and can shoot
class SpaceShipModel : public PhysicEngine
{
public:
	std::set<std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>>> bullets;

	bool isBoosted = false;
	bool isActive = true;
	bool shooted = false;
	void Move();
	
	std::pair<std::shared_ptr<BulletModel>, std::shared_ptr<AsteroidsView>> CreateBullet(asteroids::Coords pos, float rotation);
	void shoot(SoundController sound);
	SpaceShipModel();
	~SpaceShipModel();
};