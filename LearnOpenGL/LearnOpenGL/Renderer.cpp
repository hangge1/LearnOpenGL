#include "Renderer.h"



#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << " " << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
    //glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray& va, const Shader& shader, int count) const
{
    va.Bind();
    shader.Bind();

    GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}