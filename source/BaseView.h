#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace asteroids {
	struct Vector {
		float x;
		float y;
	};
	static bool isCollision(Vector pos1, float scale1, Vector pos2, float scale2) {
		//move coordinate system for collision detection

		float x = pos1.x - pos2.x;
		float y = pos1.y - pos2.y;

		float distance = sqrt((x * x) + (y * y));
		return distance <= scale1 + scale2;
	}
	class BaseView {	

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

		Vector updatePosition(Vector forward, Vector pos)
		{
			/*pos.x += forward.x * cos(rotation) - forward.y * sin(rotation);
			pos.y += forward.x * sin(rotation) + forward.y * cos(rotation);*/
			pos.x += forward.x;
			pos.y += forward.y;
			transferCoordinates(pos.x, pos.y);
			return pos;
		}
	};
	
}
