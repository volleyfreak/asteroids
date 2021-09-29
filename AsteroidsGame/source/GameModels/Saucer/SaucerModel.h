#pragma once
#include <set>

#include "../PhysicEngine.h"
#include "Bullet/BulletModel.h"
#include "View/AsteroidsView.h"

//Game object for saucers
class SaucerModel : public PhysicEngine
{
public:
	unsigned int ticks = 0;
	unsigned int score = 0;
	bool isActive = true;
	bool isSmallSaucer = false;

	std::set<std::shared_ptr<BulletModel>> bullets;

	/**
	 * Creates a bullet and adds it to set of bullets of the saucer. The bullet has a random direction and fixed speed
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	void ShootRandomBullet();

	/**
	 * Creates a bullet and adds it to set of bullets of the Spaceship. The bullet's direction is targeted to the spacecraft and it has a fixed speed
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	void ShootTargetedBullet(asteroids::Coords target);

	/**
	 * Constructor for SaucerModel game object
	 *
	 * @param score Score you get from destroying the object
	 * @param size Overwrites default size
	 * @param speed Speed in x-direction
	 *
	 */
	SaucerModel(int score, float size = 0.01f, float speed = 0.003f);
	~SaucerModel();
};