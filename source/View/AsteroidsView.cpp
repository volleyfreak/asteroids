#include "AsteroidsView.h"


AsteroidsView::AsteroidsView(GameModel* gameModel, Shader& s)
	:shader(s)
{
	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	const int size = static_cast<int>(gameModel->positions.size());
	float arr[40];
	std::copy(gameModel->positions.begin(), gameModel->positions.end(), arr);
	VertexBuffer vb(arr, 40 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(2);
	va.AddBuffer(vb, layout);
}

AsteroidsView::~AsteroidsView()
{
	va.Unbind();
	shader.Unbind();
}

void AsteroidsView::GameTick(GameModel spaceCraftModel,asteroids::Coords pos, int lines)
{
	shader.Bind();
	shader.SetUniform2f("uSize", spaceCraftModel.size, spaceCraftModel.size);
	shader.SetUniform2f("uPosition", pos.x, pos.y);
	shader.SetUniform1f("uRotate", spaceCraftModel.rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();
	GLCall(glDrawArrays(GL_LINES, 0, lines));
}
void AsteroidsView::BulletTick(GameModel* spaceCraftModel,asteroids::Coords* pos, int lines)
{
	shader.Bind();
	shader.SetUniform2f("uSize", spaceCraftModel->size, spaceCraftModel->size);
	shader.SetUniform2f("uPosition", pos->x, pos->y);
	shader.SetUniform1f("uRotate", spaceCraftModel->rotation);
	shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

	va.Bind();
	GLCall(glDrawArrays(GL_POINTS, 0, lines));
}
