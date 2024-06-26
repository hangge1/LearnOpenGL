#pragma once

#include <string>
#include <vector>

#include <glad/glad.h> 
#include <glm.hpp>

#include "Texture.h"
#include "Shader.h"

struct Vertex 
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(const Shader& shader) const;

private:
    void SetupMesh();

private:
    unsigned int VAO, VBO, EBO;
    /*  Íø¸ñÊý¾Ý  */
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;
};