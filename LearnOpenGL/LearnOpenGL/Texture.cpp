
#include "Texture.h"

#include <glad/glad.h> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& path)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width_, &height_, &nchannel_, 0);
	if (data)
	{
		unsigned int format = GetFormat();
		glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		__debugbreak();
	}

	stbi_image_free(data);
}

Texture::Texture(const Texture& rhs)
{
	Clone(rhs);
}

Texture& Texture::operator=(const Texture& rhs)
{
	Clone(rhs);

	return *this;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Clone(const Texture& rhs)
{
	type = rhs.type;

	m_ID = rhs.m_ID;
	width_ = rhs.width_;
	height_ = rhs.height_;
	nchannel_ = rhs.nchannel_;
}

unsigned int Texture::GetFormat() const
{
	unsigned int format = 0;
	if (nchannel_ == 1)
		format = GL_RED;
	else if (nchannel_ == 3)
		format = GL_RGB;
	else if (nchannel_ == 4)
		format = GL_RGBA;
	return format;
}