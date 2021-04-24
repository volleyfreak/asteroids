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
}
