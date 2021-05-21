#include "pch.h"

#include "GameModel.h"
#include "SpaceShip/SpaceShipModel.h"
#include "SpaceShip/SpaceShipModel.cpp"
#include "Asteroid/AsteroidModel.h"
#include "Asteroid/AsteroidModel.cpp"
#include "Saucer/SaucerModel.h"
#include "Saucer/SaucerModel.cpp"
#include "Utils.h"

TEST(Models, spaceShipInitializedAtZero) {
	auto spaceShip = SpaceShipModel();
	EXPECT_EQ(0.0f, spaceShip.pos.x);
	EXPECT_EQ(0.0f, spaceShip.pos.y);
}

TEST(Models, AsteroidIsInitializedAtProvidedPos) {
	auto asteroid = AsteroidModel({ 0.5f, 0.3f }, 1.0f, 0, 20, { 0.0f, 0.0f });
	asteroids::Coords pos = { 0.5f, 0.3f };
	EXPECT_EQ(pos.x, asteroid.pos.x);
	EXPECT_EQ(pos.y, asteroid.pos.y);
}

TEST(Models, SaucerIsInitializedAtScreenSide) {
	auto asteroid = SaucerModel(200);
	EXPECT_EQ(1.0f, abs(asteroid.pos.x));
}

TEST(Collision, CollisionShouldBeFalse) {
	auto spaceShip = SpaceShipModel();
	auto asteroid = AsteroidModel({ 0.5f, 0.3f }, 0.025f, 0, 20, { 0.0f, 0.0f });
	EXPECT_FALSE(asteroids::IsCollision(spaceShip.pos, spaceShip.GetCollisionRadius(), asteroid.pos, asteroid.GetCollisionRadius()));	
}

TEST(Collision, CollisionShouldBeTrue) {
	auto spaceShip = SpaceShipModel();
	auto asteroid = AsteroidModel({ 0.05f, 0.05f }, 0.025f, 0, 20, { 0.0f, 0.0f });
	EXPECT_TRUE(asteroids::IsCollision(spaceShip.pos, spaceShip.GetCollisionRadius(), asteroid.pos, asteroid.GetCollisionRadius()));
}

TEST(Moving, MovingIsAlwaysInScreenBoundary) {
	auto spaceShip = SpaceShipModel();
	spaceShip.pos = asteroids::UpdatePosition(asteroids::Coords{ 5.0f,10.0f }, spaceShip.pos);
	EXPECT_TRUE(abs(spaceShip.pos.x) <= 1.0f);
	EXPECT_TRUE(abs(spaceShip.pos.y) <= 1.0f);
}

TEST(Moving, NormalizesVector) {
	asteroids::Coords coords = { 5.0f, 5.0f };
	auto normal = asteroids::normalize(coords.x, coords.y);
	EXPECT_NEAR(sqrt(normal.x * normal.x + normal.y * normal.y), 0.015f, 0.001f);
}