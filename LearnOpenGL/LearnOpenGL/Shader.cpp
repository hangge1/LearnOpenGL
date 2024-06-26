
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
    ss << file.rdbuf(); // ���ļ����ݶ��뵽 stringstream
    return ss.str();    // ���� stringstream ��������Ϊ string
}

Shader::Shader(const std::string& vs_path, const std::string& fs_path)
    : m_ID(-1)
{
    //���롢����������ɫ��
    std::string vertexShaderSource = readFileToString(vs_path.c_str());
    const char* vs = vertexShaderSource.c_str();
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);
    //��ȡ������
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX\n" << infoLog << std::endl;
        return;
    }

    //OpenGL��GLSL�ж���һ����ɫ��ʱ�����ǰ���ɫÿ��������ǿ��������0.0��1.0֮��
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
    if (location == -1)
    {
        __debugbreak();
    }
    glUniform1i(location, v);
}

void Shader::SetUniform1f(const std::string& name, float v)
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniform1f(location, v);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3 v)
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniform3f(location, v.x, v.y, v.z);
}

void Shader::SetUniform4mat(const std::string& name, const glm::mat4& v)
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
}