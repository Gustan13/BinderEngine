#include "camera.h"

Camera::Camera() {
	this->position = glm::vec3(0.f);
	this->front = glm::vec3(0.f, 0.f, -1.f);
	this->up = glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Camera::getPosition() {
	return this->position;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::moveForward(float deltaTime) {
	this->position += this->speed * deltaTime * this->front;
}

void Camera::moveBackward(float deltaTime) {
	this->position -= this->speed * deltaTime * this->front;
}

void Camera::moveLeft(float deltaTime) {
	this->position -= glm::normalize(glm::cross(this->front, this->up)) * this->speed * deltaTime;
}

void Camera::moveRight(float deltaTime) {
	this->position += glm::normalize(glm::cross(this->front, this->up)) * this->speed * deltaTime;
}

void Camera::setDirection(float x, float z) {
	glm::vec3 direction;
	direction.x = x;
	direction.z = z;
	front = glm::normalize(direction);
}