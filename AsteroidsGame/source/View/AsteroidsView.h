#pragma once
#include "GameModels/PhysicEngine.h"
#include "constants.h"
#include "Shader.h"
#include "VertexArray.h"

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
	PhysicEngine* gameModel;
public:
	AsteroidsView(PhysicEngine* gameModel);
	~AsteroidsView();

	//Basic GameTick. Lines to draw are calculated by gameModel.bufferSize
	void Draw(Shader shader);

	// @overload void GameTick(int lines)
	void Draw(Shader shader, int lines);

	//Gametick for point objects
	void DrawBullet(Shader shader);

};