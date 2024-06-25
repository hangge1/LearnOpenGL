#pragma once

#include <string>

#include "glm.hpp"
#include "gtc/type_ptr.hpp"

class Shader
{
private:
	unsigned int m_ID;

public:
	Shader(const std::string& vs_path, const std::string& fs_path);
	~Shader();
	Shader(const Shader&) = delete;
	Shader&  operator=(const Shader&) = delete;

	void Bind();
	void UnBind();
	void SetUniform1i(const std::string& name, unsigned int v);
	void SetUniform3f(const std::string& name, const glm::vec3 v);
	void SetUniform4mat(const std::string& name, const glm::mat4& v);
};