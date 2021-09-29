#include "NumberModel.h"

NumberModel::NumberModel(int number)
	: PhysicEngine()
{
	bufferSize = 28;
	switch (number) {
	case 0:
		this->layout = Get0();
		bufferSize = 6*4;
		break;
	case 1:
		this->layout = Get1();
		bufferSize = 2 * 4;
		break;
	case 2:
		this->layout = Get2();
		bufferSize = 5 * 4;
		break;
	case 3:
		this->layout = Get3();
		bufferSize = 5 * 4;
		break;
	case 4:
		this->layout = Get4();
		bufferSize = 4 * 4;
		break;
	case 5:
		this->layout = Get5();
		bufferSize = 5 * 4;
		break;
	case 6:
		this->layout = Get6();
		bufferSize = 5 * 4;
		break;
	case 7:
		this->layout = Get7();
		bufferSize = 3 * 4;
		break;
	case 8:
		this->layout = Get8();
		bufferSize = 7 * 4;
		break;
	case 9:
		this->layout = Get9();
		bufferSize = 6 * 4;
		break;
	}
	this->rotation = 0.0f;
	this->size = 0.2f;
}

NumberModel::~NumberModel()
{
}

std::vector<float> NumberModel::Get0() {
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

std::vector<float> NumberModel::Get1() {
	return {
		0.1f, 0.2f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.0f,
	};
}

std::vector<float> NumberModel::Get2() {
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

std::vector<float> NumberModel::Get3() {
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

std::vector<float> NumberModel::Get4() {
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

std::vector<float> NumberModel::Get5() {
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

std::vector<float> NumberModel::Get6() {
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

std::vector<float> NumberModel::Get7() {
	return {
		0.1f, 0.0f,
		0.1f, 0.1f,

		0.1f, 0.1f,
		0.1f, 0.2f,

		0.1f, 0.2f,
		0.0f, 0.2f,
	};
}

std::vector<float> NumberModel::Get8() {
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

std::vector<float> NumberModel::Get9() {
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