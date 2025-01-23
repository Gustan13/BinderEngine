#include "mesh.h"

Mesh::Mesh() {
    EBO = VBO = VAO = 0;

    diffuse = glm::vec3(1.f, 0.5f, 0.31f);
    specular = glm::vec3(0.5f, 0.5f, 0.5f);
    shininess = 32.f;
}

void Mesh::setDiffuse(glm::vec3 newValue) {
	diffuse = newValue;
}
void Mesh::setSpecular(glm::vec3 newValue) {
	specular = newValue;
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), &data[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(Vertex), (void*)offsetof(Vertex, boneIDs));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void Mesh::drawMesh(Shader* shader_program) {
    shader_program->use();
    glBindVertexArray(VAO);

    //glm::mat4 model = glm::mat4(1.0f);

    for (int i = 0; i < 4; ++i)

    //shader_program->setMatrix("model", glm::value_ptr(model));
    shader_program->setVec3("material.specular", glm::value_ptr(specular));
    shader_program->setFloat("material.shininess", shininess);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES,0,36);
    glBindVertexArray(0);
}

void Mesh::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}