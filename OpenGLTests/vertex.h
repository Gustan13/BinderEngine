#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

	unsigned int boneIDs[MAX_BONE_INFLUENCE];
	float weights[MAX_BONE_INFLUENCE];
};