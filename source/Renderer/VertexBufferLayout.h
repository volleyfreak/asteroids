#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

//Element used in VertexBufferLayout
struct VertexBufferElement
{	
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	/**
	 * returns byte size of OpenGL type
	 *
	 * @param type Unsigned int representation of OpenGL types
	 */
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private: 
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	/**
	 * Set layout of data
	 *
	 * @param count Count of entries that belong together
	 */
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	
	template<>
	void Push<float>(unsigned int count) 
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	//returns m_Elements
	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	//returns m_Stride
	inline unsigned int GetStride() const { return m_Stride; }
};

