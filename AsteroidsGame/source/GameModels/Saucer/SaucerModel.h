#pragma once

#include "../GameModel.h"

//Game object for saucers
class SaucerModel : public GameModel
{
public:
	unsigned int ticks = 0;
	unsigned int score = 0;
	bool isActive = true;
	bool isSmallSaucer = false;

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