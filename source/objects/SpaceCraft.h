#pragma once

class SpaceCraft
{
private:
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
	float forward = 0.0f;
	float rotation = 0.0f;
	Position pos = { 0.1f, 0.0f };

public:
	SpaceCraft();
	~SpaceCraft();

	void Bind() const;
	void Unbind() const;

	void GameTick() const;
};