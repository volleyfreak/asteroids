#pragma once
#include "SpaceCraft/SpaceCraftModel.h"
#include "GameModel.h"
#include "Structs.h"

class AsteroidsView
{
private:
	VertexArray va;
	Shader& shader;
public:
	AsteroidsView(GameModel* gameModel, Shader& s);
	~AsteroidsView();

	void GameTick(GameModel spaceCraftModel, asteroids::Coords pos, int lines);
	void BulletTick(GameModel* spaceCraftModel, asteroids::Coords* pos, int lines);

};