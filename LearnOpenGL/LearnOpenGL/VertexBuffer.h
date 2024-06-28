#pragma once

#include <iostream>

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	bool m_IsDynamicDraw;
public:
	VertexBuffer(const void* data, unsigned int size, bool isDynamicDraw = false);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void DynamicWriteData(ptrdiff_t offset, ptrdiff_t size, const void* data) const;
};