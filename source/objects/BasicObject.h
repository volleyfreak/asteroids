#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct Position {
	float x;
	float y;
};

static void transferCoordinates(float& x, float& y)
{
	if (x > 1)
		x = -1;
	if (x < -1)
		x = 1;
	if (y > 1)
		y = -1;
	if (y < -1)
		y = 1;
}