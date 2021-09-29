#pragma once
#include "PhysicEngine.h"
#include "Utils.h"
#include <memory>
#include <set>

#include "Bullet/BulletModel.h"
#include "View/AsteroidsView.h"


//Main playing figure, a spaceship that is moved by player input and can shoot
class SpaceShipModel : public PhysicEngine
{
private:
	std::shared_ptr<BulletModel> CreateBullet(asteroids::Coords pos, float rotation);
public:
	std::set<std::shared_ptr<BulletModel>> bullets;

	bool isBoosted = false;
	bool isActive = true;
	bool shooted = false;
	
	/**
	 * Creates a bullet and adds it to set of bullets of the Spaceship. The bullet has a fixed direction and speed
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	void shoot();
	SpaceShipModel();
	~SpaceShipModel();
};