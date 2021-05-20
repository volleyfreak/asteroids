#pragma once
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
//Wrapper for OpenGL functions to break on error and provide error information
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

/**
* Wrapper for OpenGL functions
*
* @param function OpenGl function
* @param file File which made the call
* @param line Line in where the call was made
*
* @returns boolean for assertion of succesfull function call
*/
bool GLLogCall(const char* function, const char* file, int line);