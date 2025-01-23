#include "cube.h"

Cube::Cube() {
    VBO = VAO = 0;

    ambient = glm::vec3(0.3f, 0.3f, 0.3f);
    diffuse = glm::vec3(1.f, 0.5f, 0.31f);
    specular = glm::vec3(0.5f, 0.5f, 0.5f);
    shininess = 32.f;
}

Cube::~Cube() {
    this->destroy();
}

void Cube::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::setupCube() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

glm::vec3 Cube::getPosition() {
    return this->position;
}

void Cube::setPosition(glm::vec3 newPosition) {
    this->position = newPosition;
}
void Cube::setRotation(glm::vec3 newRotation) {
    this->rotation = newRotation;
}
void Cube::setScale(glm::vec3 newScale) {
    this->scale = newScale;
}

void Cube::setAmbient(glm::vec3 newValue) {
    ambient = newValue;
}
void Cube::setDiffuse(glm::vec3 newValue) {
    diffuse = newValue;
}
void Cube::setSpecular(glm::vec3 newValue) {
    specular = newValue;
}

void Cube::drawCube(Shader* shader_program) {
    glm::mat4 model;

    glBindVertexArray(VAO);

    model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    shader_program->setMatrix("model", glm::value_ptr(model));
    shader_program->setVec3("material.specular", glm::value_ptr(specular));
    shader_program->setFloat("material.shininess", shininess);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}