#pragma once

#include <string>
#include <vector>

#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
    Model(const std::string& path);
  
    void Draw(const Shader& shader);

private:
    /*  函数   */
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
    /*  模型数据  */
    std::vector<Mesh> meshes_;
    std::string directory_;
};