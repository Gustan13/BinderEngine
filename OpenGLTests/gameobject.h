#pragma once

#include <glm/glm.hpp>
#include <string>

#include "model.h"

class GameObject {
public:
	glm::vec3 position, rotation, scale = glm::vec3(0.01f,0.01f,0.01f), ellipseDims = glm::vec3(1.f, 1.f, 1.f);
	Model* currentModel = nullptr;
	std::string modelName;

	bool canInteract, canCollide, playingAnimation;
	float animTimer = 0.0f;

	void playAnimation();
	void stopAnimation();
	virtual void update() {};
};