
#include "Shader.h"

#include <glad/glad.h> 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

static std::string readFileToString(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file)
    {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::ostringstream ss;
    ss << file.rdbuf(); // 将文件内容读入到 stringstream
    return ss.str();    // 返回 stringstream 的内容作为 string
}

Shader::Shader(const std::string& vs_path, const std::string& fs_path)
    : m_ID(-1)
{
    //编译、链接生成着色器
    std::string vertexShaderSource = readFileToString(vs_path.c_str());
    const char* vs = vertexShaderSource.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);
    //获取编译结果
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX\n" << infoLog << std::endl;
        return;
    }

    //OpenGL或GLSL中定义一个颜色的时候，我们把颜色每个分量的强度设置在0.0到1.0之间
    std::string fragmentShaderSource = readFileToString(fs_path.c_str());
    const char* fs = fragmentShaderSource.c_str();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT\n" << infoLog << std::endl;
        return;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM\n" << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(m_ID);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Bind()
{
    glUseProgram(m_ID);
}


void Shader::UnBind()
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, unsigned int v)
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    glUniform1i(location, v);
}