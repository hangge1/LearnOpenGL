#pragma once

class FrameBuffer
{
public:
	FrameBuffer(int width, int height, bool useRenderBufferAttachment = true);
	~FrameBuffer();

	void Bind() const;
	void UnBind() const;

	void BindColorTexture(unsigned int slot = 0) const;

private:
	unsigned int m_ID;
	unsigned int colorTextureID;
	unsigned int depthStencilID;
	int width_;
	int height_;
	bool useRenderBufferAttachment_;
};