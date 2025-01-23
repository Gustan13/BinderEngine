#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node {
public:
	glm::mat4 transformation;
	std::vector<unsigned int> meshes;
	std::vector<Node*> subnodes;
};