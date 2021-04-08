#include "Asteroid.h"
#pragma once

Asteroid::Asteroid(Shader& s)
	:shader{ s }, pos{ randomF(), randomF() }, rotation (randomF(1.5f)), forward( randomF(0.005f))
{
}

Asteroid::~Asteroid()
{
}

void Asteroid::Bind()
{

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	VertexBuffer vb(positions, 4 * 10 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);

	shader.Bind();
}

void Asteroid::Unbind()
{
	va.Unbind();
	shader.Unbind();
}

void Asteroid::GameTick()
{
	updatePosition(forward, rotation, pos);

	shader.Bind();
	shader.SetUniform2f("uSize", 0.03f, 0.03f);
	shader.SetUniform2f("uPosition", pos.x, pos.y);
	shader.SetUniform1f("uRotate", rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();

	GLCall(glDrawArrays(GL_LINES, 0, 20));
}