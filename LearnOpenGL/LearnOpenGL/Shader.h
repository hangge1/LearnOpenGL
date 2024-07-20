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
	Shader(const std::string& vs_path, const std::string& fs_path, const std::string& gs_path);
	~Shader();
	Shader(const Shader&) = delete;
	Shader&  operator=(const Shader&) = delete;

	void Bind() const;
	void UnBind() const;
	void SetUniform1i(const std::string& name, unsigned int v) const;
	void SetUniform1f(const std::string& name, float v) const;
	void SetUniform2f(const std::string& name, glm::vec2 v) const;
	void SetUniform3f(const std::string& name, const glm::vec3 v) const;
	void SetUniform4mat(const std::string& name, const glm::mat4& v) const;

	unsigned int GetUniformBlockIndex(const std::string& block_name) const;
	void SetUniformBlockBinding(const std::string& block_name, unsigned int bindIndex) const;
};