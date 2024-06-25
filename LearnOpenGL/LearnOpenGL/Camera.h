#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

class Camera
{
public:
	enum KEYMODE
	{
		None = 0,
		KEY_W = 1,
		KEY_S = 2,
		KEY_A = 4,
		KEY_D = 8
	};

	Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
	~Camera();

	void MouseCB(double xpos, double ypos);

	void ScrollCB(double offset);

	void KeyCB(float deltaTime, Camera::KEYMODE key);

	glm::mat4 GetViewMatrix();

	float GetFov() const { return fov; }
private:
	glm::vec3 cameraPos_;
	glm::vec3 cameraFront_;
	glm::vec3 cameraUp_;

	float pitch = 0.0f;
	float yaw = -90.0f;

	float fov = 20.0f;

	bool firstMouse = true;
	float lastX;
	float lastY;

	float cameraMoveSpeed = 2.0f;
	float mouseSensitivity = 0.1f;
};