#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "shaders.h"
#include "vertex.h"

class Mesh {
public:
	unsigned int VBO, EBO, VAO;

	//std::vector<float> data;
	std::vector<Vertex> data;
	std::vector<unsigned int> indices;
	glm::vec3 diffuse, specular;
	float shininess;

	Mesh();

	void setDiffuse(glm::vec3 newValue);
	void setSpecular(glm::vec3 newValue);

	void setupMesh();
	void drawMesh(Shader* shader_program);

	void destroy();
};