#pragma once

#include <string>

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
};