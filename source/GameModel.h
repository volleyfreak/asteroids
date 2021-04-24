#pragma once
#include "Structs.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

//struct ModelView {
//	GameModel* gameModel;
//	SpaceCraftView* view;
//};
class GameModel
{
private:

public:
	std::vector<float> positions;
	int bufferSize = 20;
	asteroids::Vector forward = { 0.0f, 0.0f };
	float rotation = 0.0f;
	float size = 0.0f;
	asteroids::Vector pos = { 0.0f, 0.0f };
	GameModel(int positions) : bufferSize(positions) {}
};