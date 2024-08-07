#pragma once

#include <glad/glad.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:

public:
    void Clear() const;
    //带EBO的渲染
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    //不带EBO的渲染
    void Draw(const VertexArray& va, const Shader& shader, int count) const;

    //Instanced渲染
    void DrawInstanced(const VertexArray& va, const Shader& shader, int count, int instanceCount) const;
};