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
	GameModel* gameModel;
public:
	AsteroidsView(GameModel* gameModel, Shader& s);
	~AsteroidsView();

	void GameTick();
	void GameTick(int lines);
	void BulletTick();

};