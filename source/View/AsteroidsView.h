#pragma once
#include "GameModels/GameModel.h"
#include "Shader.h"
#include "Structs.h"
#include "VertexArray.h"
#include "Shader.h"

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