#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

//Utils
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


	/**
	 * Sums up two vectors. Value is transvestited to other side if reaching end of screen.
	 *
	 * @param forward vector1
	 * @param forward vector2
	 *
	 * @return Sum of two vectors 
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	static asteroids::Coords UpdatePosition(const asteroids::Coords& forward, asteroids::Coords pos)
	{
		pos.x += forward.x;
		pos.y += forward.y;
		if (pos.x > 1)
			pos.x = -1;
		if (pos.x < -1)
			pos.x = 1;
		if (pos.y > 1)
			pos.y = -1;
		if (pos.y < -1)
			pos.y = 1;
		return pos;
	}

	/**
	 * Determines collision of two objects
	 *
	 * @param pos1, pos2 Positions of the two objects
	 * @param scale1, scale2 Dimensions of the two objects
	 *
	 * @return boolean expression about collision
	 *
	 * @exceptsafe This function does not throw exceptions.
	 */
	static bool IsCollision(asteroids::Coords pos1, float scale1, asteroids::Coords pos2, float scale2) 
	{
		float x = pos1.x - pos2.x;
		float y = pos1.y - pos2.y;
		float distance = sqrt((x * x) + (y * y));
		return distance <= scale1 + scale2;
	}

	/**
	 * Normalizes a vector to length 0.015f
	 *
	 * @param x, y coords of vector
	 *
	 * @return the normalized vector
	 *
	 */
	static asteroids::Coords normalize(float x, float y)
	{
		float factor = 1 / sqrt((x * x) + (y * y));
		return { x * factor * 0.015f, y * factor * 0.015f };
	}	
}
