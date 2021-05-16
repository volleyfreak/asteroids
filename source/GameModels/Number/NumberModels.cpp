#include "NumberModels.h"

NumberModels::NumberModels(int number, asteroids::Coords pos)
	: GameModel()
{
	bufferSize = 28;
	switch (number) {
	case 0:
		this->positions = Get0();
		break;
	case 1:
		this->positions = Get1();
		break;
	case 2:
		this->positions = Get2();
		break;
	case 3:
		this->positions = Get3();
		break;
	case 4:
		this->positions = Get4();
		break;
	case 5:
		this->positions = Get5();
		break;
	case 6:
		this->positions = Get6();
		break;
	case 7:
		this->positions = Get7();
		break;
	case 8:
		this->positions = Get8();
		break;
	case 9:
		this->positions = Get9();
		break;
	}
	this->pos = pos;
	this->rotation = 0.0f;
	this->size = 0.2f;
}

NumberModels::~NumberModels()
{
}

std::vector<float> NumberModels::Get0() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.0f, 0.1f,

		0.0f, 0.1f,
		0.0f, 0.0f,
	};
}

std::vector<float> NumberModels::Get1() {
	return {
		0.1f, 0.2f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.0f,
	};
}

std::vector<float> NumberModels::Get2() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,

		0.0f, 0.1f,
		0.0f, 0.0f,

		0.0f, 0.1f,
		0.1f, 0.1f,
	};
}

std::vector<float> NumberModels::Get3() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,

		0.0f, 0.1f,
		0.1f, 0.1f,
	};
}

std::vector<float> NumberModels::Get4() {
	return {

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.0f, 0.2f,
		0.0f, 0.1f,

		0.0f, 0.1f,
		0.1f, 0.1f,
	};
}

std::vector<float> NumberModels::Get5() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.2f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.0f, 0.1f,

		0.0f, 0.1f,
		0.1f, 0.1f,
	};
}

std::vector<float> NumberModels::Get6() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.0f, 0.2f,
		0.0f, 0.1f,

		0.0f, 0.1f,
		0.0f, 0.0f,

		0.0f, 0.1f,
		0.1f, 0.1f,
	};
}

std::vector<float> NumberModels::Get7() {
	return {
		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,
	};
}

std::vector<float> NumberModels::Get8() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.0f, 0.1f,

		0.0f, 0.1f,
		0.0f, 0.0f,

		0.0f, 0.1f,
		0.1f, 0.1f, 
	};
}

std::vector<float> NumberModels::Get9() {
	return {
		0.0f, 0.0f,
		0.1f, 0.0f,

		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,

		0.0f, 0.2f,
		0.0f, 0.1f,

		0.0f, 0.1f,
		0.1f, 0.1f,
	};
}