#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace asteroids {
	struct Coords {
		float x;
		float y;
	};	
	static int flip = 0;
	static float randomF(float modify = 1.0f) {
		auto randomValue = ((float)rand() / ((float)RAND_MAX)) * modify;
		if (flip == 2) {			
			flip = 0;
			return randomValue * -1.0f;
		}
		else {
			flip++;
			return randomValue;
		}
		
	}	

	static void TransferCoordinates(float& x, float& y)
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

	static asteroids::Coords* UpdatePosition(asteroids::Coords* forward, asteroids::Coords* pos)
	{
		/*pos.x += forward.x * cos(rotation) - forward.y * sin(rotation);
		pos.y += forward.x * sin(rotation) + forward.y * cos(rotation);*/
		pos->x += forward->x;
		pos->y += forward->y;
		TransferCoordinates(pos->x, pos->y);
		return pos;
	}

	static bool IsCollision(asteroids::Coords pos1, float scale1, asteroids::Coords pos2, float scale2) {
		//move coordinate system for collision detection

		float x = pos1.x - pos2.x;
		float y = pos1.y - pos2.y;

		float distance = sqrt((x * x) + (y * y));
		return distance <= scale1 + scale2;
	}
}
