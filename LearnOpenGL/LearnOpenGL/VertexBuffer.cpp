
#include "Renderer.h"


#include "VertexBuffer.h"



VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool isDynamicDraw)
    : m_IsDynamicDraw(isDynamicDraw)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    if (isDynamicDraw)
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
    }
    else
    {
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW));
    }

}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::DynamicWriteData(ptrdiff_t offset, ptrdiff_t size, const void* data) const
{
    if (!m_IsDynamicDraw)
    {
        return;
    }
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}