#include "pch.h"

#include <cmath>
#include <list>
#include <set>
#include <iostream>
#include <memory>

#include "PhysicEngine.h"
#include "SpaceShip/SpaceShipModel.h"
#include "SpaceShip/SpaceShipModel.cpp"
#include "Asteroid/AsteroidModel.h"
#include "Asteroid/AsteroidModel.cpp"
#include "Saucer/SaucerModel.h"
#include "Saucer/SaucerModel.cpp"
#include "Bullet/BulletModel.h"
#include "Bullet/BulletModel.cpp"
#include "Utils.h"
#include "constants.h"


TEST(Models, spaceShipInitializedAtZero) {
	auto spaceShip = SpaceShipModel();
	EXPECT_EQ(0.0f, spaceShip.position.x);
	EXPECT_EQ(0.0f, spaceShip.position.y);
}

TEST(Models, AsteroidIsInitializedAtProvidedPos) {
	auto asteroid = AsteroidModel({ 0.5f, 0.3f }, 1.0f, 0, 20, { 0.0f, 0.0f });
	asteroids::Coords pos = { 0.5f, 0.3f };
	EXPECT_EQ(pos.x, asteroid.position.x);
	EXPECT_EQ(pos.y, asteroid.position.y);
}

TEST(Models, SaucerIsInitializedAtScreenSide) {
	auto asteroid = SaucerModel(200);
	EXPECT_EQ(1.0f, abs(asteroid.position.x));
}

TEST(Collision, CollisionShouldBeFalse) {
	auto spaceShip = SpaceShipModel();
	auto asteroid = AsteroidModel({ 0.5f, 0.3f }, 0.025f, 0, 20, { 0.0f, 0.0f });
	EXPECT_FALSE(spaceShip.IsCollision(asteroid));	
}

TEST(Collision, CalculateImpactShouldGiveDifferentResults) {
	auto asteroid = AsteroidModel({ 0.5f, 0.3f }, 0.025f, 0, 20, { 0.5f, 0.2f });
	auto result1 = asteroid.CalculateImpact(BULLET_IMPACT);
	auto result2 = asteroid.CalculateImpact(BULLET_IMPACT);

	EXPECT_FALSE(result1.x == result2.x && result1.y == result2.y);
}

TEST(Collision, CollisionShouldBeTrue) {
	auto spaceShip = SpaceShipModel();
	auto asteroid = AsteroidModel({ 0.05f, 0.05f }, 0.025f, 0, 20, { 0.0f, 0.0f });
	EXPECT_TRUE(spaceShip.IsCollision(asteroid));
}

TEST(Moving, MovingIsAlwaysInScreenBoundary) {
	auto spaceShip = SpaceShipModel();
	spaceShip.position = spaceShip.UpdatePosition(asteroids::Coords{ 5.0f,10.0f }, spaceShip.position);
	EXPECT_TRUE(abs(spaceShip.position.x) <= 1.0f);
	EXPECT_TRUE(abs(spaceShip.position.y) <= 1.0f);
}

TEST(Moving, NormalizesVector) {
	auto spaceShip = SpaceShipModel();
	asteroids::Coords coords = { 5.0f, 5.0f };
	auto normal = spaceShip.normalize(coords.x, coords.y);
	EXPECT_NEAR(sqrt(normal.x * normal.x + normal.y * normal.y), 0.015f, 0.001f);
}