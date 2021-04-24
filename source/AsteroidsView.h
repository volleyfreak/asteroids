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

	void GameTick(GameModel* spaceCraftModel, asteroids::Vector* pos, int lines);

};