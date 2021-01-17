#pragma once

#include "VertexBuffer.h"
//#include "VertexBufferLayout.h"		Not included because it includes "Renderer.h" but "Renderer.h" includes this file as well
//Solution:
class VertexBufferLayout;
// + include it in .cpp file


class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};