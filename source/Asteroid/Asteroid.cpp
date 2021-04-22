#include "Asteroid.h"
#pragma once

Asteroid::Asteroid(Shader& s, float direction)
	:shader{ s }, pos{ randomF(), randomF() }, rotation(0.0f), forward{ randomF(direction), randomF(direction) }
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
	pos = updatePosition(forward, pos);

	shader.Bind();
	shader.SetUniform2f("uSize", size, size);
	shader.SetUniform2f("uPosition", pos.x, pos.y);
	shader.SetUniform1f("uRotate", rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();

	GLCall(glDrawArrays(GL_LINES, 0, 20));
}