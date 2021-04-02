#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <math.h>

#include "constants/constants.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"




struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) 
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else 
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);


	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDetachShader(program, vs);
	glDetachShader(program, fs);

	return program;
}

//template<typename Scalar, std::enable_if_t<std::is_arithmetic_v<Scalar>, int> = 0> //todo: what is a template
//struct Vector2
//{
//	Scalar x = 0;
//	Scalar y = 0;
//
//public:
//	constexpr Vector2 operator*=(Scalar s) const noexcept
//	{
//		x *= s;
//		y *= s;
//
//		return *this;
//	}
//
//	constexpr Vector2 operator/=(Scalar s) const noexcept
//	{
//		x /= s;
//		y /= s;
//
//		return *this;
//	}
//
//	constexpr Vector2& operator+=(const Vector2& other) noexcept
//	{
//		x += other.x;
//		y += other.y;
//
//		return *this;
//	}

	/*LEPONG_NODISCARD constexpr Vector2 operator*(Scalar s) const noexcept
	{
		return { x * s, y * s };
	}

	LEPONG_NODISCARD constexpr Vector2 operator/(Scalar s) const noexcept
	{
		return { x / s, y / s };
	}

	LEPONG_NODISCARD constexpr Vector2 operator-() const noexcept
	{
		return { -x, -y };
	}

	LEPONG_NODISCARD constexpr Vector2 operator+(const Vector2& other) const noexcept
	{
		return { x + other.x, y + other.y };
	}

	LEPONG_NODISCARD constexpr Vector2 operator-(const Vector2& other) const noexcept
	{
		return { x - other.x, y - other.y };
	}

public:
	LEPONG_NODISCARD constexpr float Mag() const noexcept
	{
		const auto kSqMag = (float)SquareMag();
		return sqrtf(kSqMag);
	}

	LEPONG_NODISCARD constexpr Scalar SquareMag() const noexcept
	{
		return (x * x) + (y * y);
	}*/
//};

void updateInput(GLFWwindow* window, float& forward, float& rotation)
{
	bool lockPosition;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		forward += 0.0001f;
		lockPosition = false;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		forward -= 0.0001f;
		lockPosition = false;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotation -= 0.05f;
		lockPosition = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
	{
		rotation += 0.05f;
		lockPosition = true;
	}

	if (forward > 1)
		forward = -1;
	if (forward < -1)
		forward = 1;
}

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

struct Position {
	float x;
	float y; 
};

void updatePosition(float& forward, float& rotation, Position& pos)
{
	pos.x += forward * cos(rotation) - 0.0f * sin(rotation);
	pos.y += forward * sin(rotation) + 0.0f * cos(rotation);
	transferCoordinates(pos.x, pos.y);
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(ASTEROIDS_WIN_HEIGHT,  ASTEROIDS_WIN_WIDTH, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	//float positions[8] = {
	//	-0.5f, -0.5,  // 0
	//	-0.5f,  0.5f, // 1
	//	 0.5f,  0.5f, // 2
	//	 0.5f, -0.5f, // 3
	//};
	/*float positions[20] = {
		-0.25f, -0.05f,
		0.5f,  0.0f,

		0.0f, -0.2f,
		0.5f,  0.0f,

		0.0f,  -0.2f,
		0.05f, -0.1f,

		0.0f,  0.2f,
		0.05f, 0.1f,

		0.05f, 0.1f,
		0.05f, -0.1f,
	};*/

	float positions[20] = {
		-0.25f, 0.2f,  
		0.25f,  0.0f, 

		-0.25f, -0.2f,
		0.25f,  0.0f,

		-0.25f,  -0.2f,
		-0.20f, -0.1f,

		-0.25f,  0.2f,
		-0.20f, 0.1f,

		-0.20f, 0.1f,
		-0.20f, -0.1f,
	};

	/*unsigned int indices[] = {
		0, 1, 
		0, 1,
		1, 2
	};*/

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray va;
	VertexBuffer vb(positions, 4 * 5 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));
	
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);

	float forward = 0.0f;
	float rotation = 0.0f;
	Position pos = { 0.1f, 0.0f };

	std::cout << glGetString(GL_VERSION) << std::endl;


	/* L oop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		updateInput(window, forward, rotation);
		//transferCoordinates(x);
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));  

		GLCall(const auto kSizeLocation = glGetUniformLocation(shader, "uSize"));
		GLCall(glUniform2f(kSizeLocation, 0.3f, 0.3f));

		updatePosition(forward, rotation, pos);

		GLCall(const auto kPositionLocation = glGetUniformLocation(shader, "uPosition"));
		GLCall(glUniform2f(kPositionLocation, pos.x, pos.y));
		
		GLCall(const auto kRotateLocation = glGetUniformLocation(shader, "uRotate"));
		GLCall(glUniform1f(kRotateLocation, rotation));


		/*GLCall(const auto kRotationLocation = glGetUniformLocation(shader, "uRotation"));
		glUniform2f(kRotationLocation, a[0], yR);*/

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		GLCall(glUniform4f(location, 1.0f, 1.0f, 1.0f, 1.0f));


		va.Bind();
		GLCall(glDrawArrays(GL_LINES, 0, 10));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}