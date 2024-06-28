#pragma once

#include <string>
#include <vector>


class CubeTexture
{
public:
	CubeTexture(const std::vector<std::string>& faces_path);
	~CubeTexture();

	CubeTexture(const CubeTexture& rhs) = delete;
	CubeTexture& operator=(const CubeTexture&) = delete;

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	std::vector<std::string> path_;
private:
	unsigned int GetFormat(int channel) const;
private:
	unsigned int m_ID;
	std::vector<int> width_;
	std::vector<int> height_;
	std::vector<int> nchannel_;
};