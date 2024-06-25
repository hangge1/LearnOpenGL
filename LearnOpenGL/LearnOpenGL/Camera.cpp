#pragma once

#include "Camera.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp)
	: cameraPos_(cameraPos), cameraFront_(cameraFront), cameraUp_(cameraUp)
{

}

Camera::~Camera()
{

}

void Camera::MouseCB(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = (float)xpos;
	lastY = (float)ypos;

	
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront_ = glm::normalize(front);
}

void Camera::ScrollCB(double offset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (float)offset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void Camera::KeyCB(float deltaTime, Camera::KEYMODE key)
{
	float speed = cameraMoveSpeed * deltaTime;

	if (key & Camera::KEYMODE::KEY_W)
	{
		cameraPos_ += speed * cameraFront_;
	}
	if (key & Camera::KEYMODE::KEY_S)
	{
		cameraPos_ -= speed * cameraFront_;
	}
	if (key & Camera::KEYMODE::KEY_A)
	{
		cameraPos_ -= glm::normalize(glm::cross(cameraFront_, cameraUp_)) * speed;
	}
	if (key & Camera::KEYMODE::KEY_D)
	{
		cameraPos_ += glm::normalize(glm::cross(cameraFront_, cameraUp_)) * speed;
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
}