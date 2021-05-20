#pragma once
#include "GameModels/GameModel.h"
#include "constants.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Shader.h"

/**
 * View implementation of the game
 *
 * For each gameobject you have to create an instance of this class and pass the pointer to the corresping gameModel.
 * 
 */
class AsteroidsView
{
private:
	VertexArray va;
	Shader& shader;
	GameModel* gameModel;
public:
	AsteroidsView(GameModel* gameModel, Shader& s);
	~AsteroidsView();

	//Basic GameTick. Lines to draw are calculated by gameModel.bufferSize
	void GameTick();

	// @overload void GameTick(int lines)
	void GameTick(int lines);

	//Gametick for point objects
	void BulletTick();

};