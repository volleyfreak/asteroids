#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

void updateInput(GLFWwindow* window, float& x, float& y)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) 
		y += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) 
		y -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
		x += 0.01f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) 
		x -= 0.01f;
}

void transferCoordinates(float& x, float& y) 
{
	if (y > 1)
		y = -1;
	if (y < -1)
		y = 1;
	if (x > 1)
		x = -1;
	if (x < -1)
		x = 1;
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

	float positions[8] = {
		-0.5f, -0.5,  // 0
		-0.5f,  0.5f, // 1
		 0.5f,  0.5f, // 2
		 0.5f, -0.5f, // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexArray va;
	VertexBuffer vb(positions, 4 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	IndexBuffer ib(indices, 6);

	ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));
	
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	//GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

	

	/*const auto kPositionLocation = gl::GetUniformLocation(program, "uPosition");
	gl::Uniform2f(kPositionLocation, position.x, position.y);*/


	/*GLCall(glBindVertexArray(0));
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));*/
	

	float r = 0.0f;
	float increment = 0.05f;

	float x = 0.0f;
	float y = 0.0f;

	std::cout << glGetString(GL_VERSION) << std::endl;


	/* L oop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		updateInput(window, x, y);
		transferCoordinates(x, y);
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		GLCall(glUseProgram(shader));  

		GLCall(const auto kSizeLocation = glGetUniformLocation(shader, "uSize"));
		GLCall(glUniform2f(kSizeLocation, 0.3f, 0.3f));

		GLCall(const auto kPositionLocation = glGetUniformLocation(shader, "uPosition"));
		glUniform2f(kPositionLocation, x, y);

		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		va.Bind();
		ib.Bind();	
		
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		/*if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;

		r += increment;*/

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}