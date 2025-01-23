#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	glm::vec3 position, front, up;
	glm::mat4 view;

	float speed = 1.5f;
public:
	float yaw = -90.f;

	Camera();

	glm::vec3 getPosition();
	glm::mat4 getViewMatrix();
	
	void setDirection(float x, float z);

	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
};