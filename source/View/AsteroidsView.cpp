#include "AsteroidsView.h"


AsteroidsView::AsteroidsView(GameModel* gameModel, Shader& s)
	:shader(s)
{
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	this->gameModel = gameModel;

	float arr[44]; //max size of game object positions. There seems to be no way to determine it dynamically during runtime in C++;
	std::copy(gameModel->positions.begin(), gameModel->positions.end(), arr);
	VertexBuffer vb(arr, 44 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
}

AsteroidsView::~AsteroidsView()
{
	va.Unbind();
	shader.Unbind();
}

void AsteroidsView::GameTick()
{
	shader.Bind();
	shader.SetUniform2f("uSize", gameModel->size * ASTEROIDS_SCALING_X, gameModel->size);
	shader.SetUniform2f("uPosition", gameModel->pos.x, gameModel->pos.y);
	shader.SetUniform1f("uRotate", gameModel->rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();
	GLCall(glDrawArrays(GL_LINES, 0, gameModel->bufferSize / 2));
}

void AsteroidsView::GameTick(int lines)
{
	shader.Bind();
	shader.SetUniform2f("uSize", gameModel->size * ASTEROIDS_SCALING_X, gameModel->size);
	shader.SetUniform2f("uPosition", gameModel->pos.x, gameModel->pos.y);
	shader.SetUniform1f("uRotate", gameModel->rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();
	GLCall(glDrawArrays(GL_LINES, 0, lines));
}

void AsteroidsView::BulletTick()
{
	shader.Bind();
	shader.SetUniform2f("uSize", gameModel->size * ASTEROIDS_SCALING_X, gameModel->size);
	shader.SetUniform2f("uPosition", gameModel->pos.x, gameModel->pos.y);
	shader.SetUniform1f("uRotate", gameModel->rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();
	GLCall(glDrawArrays(GL_POINTS, 0, gameModel->bufferSize / 2));
}
