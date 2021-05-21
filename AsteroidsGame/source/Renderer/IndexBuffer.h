#pragma once

class IndexBuffer
{
private: 
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const void* data, unsigned int size);
	~IndexBuffer();

	//Bind IndexBuffer
	void Bind() const;

	//Unbind IndexBuffer
	void Unbind() const;

	//returns m_Count
	inline unsigned int GetCount() const { return m_Count; }
};

