#pragma once

#include <string>



class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	Texture(const Texture& rhs);
	Texture& operator=(const Texture&) = delete;

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	std::string type;
private:
	unsigned int GetFormat() const;
private:
	unsigned int m_ID;
	int width_;
	int height_;
	int nchannel_;
};