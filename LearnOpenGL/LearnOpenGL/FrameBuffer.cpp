
#include "FrameBuffer.h"

#include <glad/glad.h>

#include <iostream>

FrameBuffer::FrameBuffer(int width, int height, bool useRenderBufferAttachment)
	: width_(width), height_(height), useRenderBufferAttachment_(useRenderBufferAttachment)
{
    glGenFramebuffers(1, &m_ID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    //绑定颜色纹理附件
    glGenTextures(1, &colorTextureID);
    glBindTexture(GL_TEXTURE_2D, colorTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);

    //绑定深度和模板缓冲附件
    if (useRenderBufferAttachment_)  //法2: 渲染缓冲对象附件
    {
        glGenRenderbuffers(1, &depthStencilID);
        glBindRenderbuffer(GL_RENDERBUFFER, depthStencilID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilID);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else  //法1: 纹理附件
    {
        glGenTextures(1, &depthStencilID);
        glBindTexture(GL_TEXTURE_2D, depthStencilID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width_, height_, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilID, 0);
    }
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        __debugbreak();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

FrameBuffer::~FrameBuffer()
{
    if (useRenderBufferAttachment_)
    {
        glDeleteRenderbuffers(1, &depthStencilID);
    }
    else
    {
        glDeleteTextures(1, & depthStencilID);
    }

    glDeleteTextures(1, &colorTextureID);
    glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FrameBuffer::UnBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindColorTexture(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, colorTextureID);
}