#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;
public: 
	VertexArray();
	~VertexArray();

	/** 
	* Combines VertexBuffer game object positions and VertexBufferLayout information how the buffer is structured to make OpenGL calls
	* Creates GlVertexAttribPointer for every element in buffer.
	*
	* @param vb VertexBuffer with initialized positions of game object
	* @param layout VertexBufferLayout with information about how data in VertexBuffer is saved.
	* 
	*/
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	//Bind VertexArray
	void Bind() const;

	//Unbind VertexArray
	void Unbind() const;
};

