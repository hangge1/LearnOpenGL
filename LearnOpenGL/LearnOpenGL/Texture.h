#pragma once

#include <string>



class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;
private:
	unsigned int m_ID;
	int width_;
	int height_;
	int nchannel_;
};