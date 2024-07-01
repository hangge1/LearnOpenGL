
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

Shader::Shader(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path)
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

    std::string geometryShaderSource = readFileToString(gs_path.c_str());
    const char* gs = geometryShaderSource.c_str();
    unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &gs, NULL);
    glCompileShader(geometryShader);
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        std::cout << "ERROR::GEOMETRY\n" << infoLog << std::endl;
        return;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glAttachShader(m_ID, geometryShader);

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
    glDeleteShader(geometryShader);
    glUseProgram(m_ID);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

void Shader::Bind() const
{
    glUseProgram(m_ID);
}


void Shader::UnBind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, unsigned int v) const
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniform1i(location, v);
}

void Shader::SetUniform1f(const std::string& name, float v) const
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniform1f(location, v);
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3 v) const
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniform3f(location, v.x, v.y, v.z);
}

void Shader::SetUniform4mat(const std::string& name, const glm::mat4& v) const
{
    int location = glGetUniformLocation(m_ID, name.c_str());
    if (location == -1)
    {
        __debugbreak();
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(v));
}

unsigned int Shader::GetUniformBlockIndex(const std::string& block_name) const
{
    return glGetUniformBlockIndex(m_ID, block_name.c_str());
}

void Shader::SetUniformBlockBinding(const std::string& block_name, unsigned int bindIndex) const
{
    unsigned int index = GetUniformBlockIndex(block_name);

    glUniformBlockBinding(m_ID, index, bindIndex);
}