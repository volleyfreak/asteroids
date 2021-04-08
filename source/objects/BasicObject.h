#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace asteroids {
	struct Position {
		float x;
		float y;
	};
	class BasicObject {	

	public: 
		void transferCoordinates(float& x, float& y)
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

		static float randomF(float modify = 1.0f) {			
			return ((float)rand() / (float)RAND_MAX) * modify;
		}

		Position updatePosition(float& forward, float& rotation, Position& pos)
		{
			pos.x += forward * cos(rotation) - 0.0f * sin(rotation);
			pos.y += forward * sin(rotation) + 0.0f * cos(rotation);
			transferCoordinates(pos.x, pos.y);
			return pos;
		}
	};
	
}
