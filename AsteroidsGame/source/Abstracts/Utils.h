#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

//Utils for whole project
namespace asteroids {

	// Struct of two float number: x and y. Used to bind positions and vectors.
	struct Coords {
		float x;
		float y;
	};	

	static int flip = 0;

	/**
	 * Gets a Random float number between 0 and max
	 *
	 * @param max Max size of random number. Default is 1.0f
	 *
	 * @return float Random number as float
	 *
	 */
	static float randomF(float max = 1.0f) {
		auto randomValue = ((float)rand() / ((float)RAND_MAX)) * max;
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
