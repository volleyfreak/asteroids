#pragma once

class VertexBuffer
{
private: 
	unsigned int m_RendererID;
public:

	/**
	 * Initializes VertexBuffer
	 *
	 * @param data Position data in basic form like float array
	 * @param size Size of array
	 */
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	//Binds VertexBuffer
	void Bind() const;

	//Unbinds VertexBuffer
	void Unbind() const;
};

