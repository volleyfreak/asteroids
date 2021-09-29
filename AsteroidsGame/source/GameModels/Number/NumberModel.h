#pragma once

#include "../PhysicEngine.h"

//Game object for numbers
class NumberModel : public PhysicEngine
{
private:
	std::vector<float> Get0();
	std::vector<float> Get1();
	std::vector<float> Get2();
	std::vector<float> Get3();
	std::vector<float> Get4();
	std::vector<float> Get5();
	std::vector<float> Get6();
	std::vector<float> Get7();
	std::vector<float> Get8();
	std::vector<float> Get9();
public:

	/**
	 * Constructor for SaucerModel game object
	 *
	 * @param number Provide number between 0 and 9 that you want to draw
	 * @param pos Position where to draw the number
	 *
	 */
	NumberModel(int number);
	~NumberModel();
};

