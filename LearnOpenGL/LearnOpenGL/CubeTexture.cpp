
#include "CubeTexture.h"

#include <iostream>

#include <glad/glad.h> 

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CubeTexture::CubeTexture(const std::vector<std::string>& faces_path)
	: path_(faces_path)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < path_.size(); i++)
	{
		unsigned char* data = stbi_load(path_[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			unsigned int format = GetFormat(nrChannels);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << path_[i] << std::endl;
			__debugbreak();
		}
		stbi_image_free(data);

		width_.push_back(width);
		height_.push_back(height);
		nchannel_.push_back(nrChannels);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeTexture::~CubeTexture()
{
	glDeleteTextures(1, &m_ID);
}

void CubeTexture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void CubeTexture::UnBind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


unsigned int CubeTexture::GetFormat(int channel) const
{
	unsigned int format = 0;
	if (channel == 1)
		format = GL_RED;
	else if (channel == 3)
		format = GL_RGB;
	else if (channel == 4)
		format = GL_RGBA;
	return format;
}