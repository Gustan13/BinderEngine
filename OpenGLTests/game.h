#pragma once

//#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "shaders.h"
#include "camera.h"
#include "cube.h"
#include "texture.h"

#include "model.h"
#include "modeltable.h"
#include "gameobject.h"

class Game {
private:
    GLFWwindow* window;
    Camera* camera;
    Cube *lightSource;
    Shader *litShader, *lightShader;
    Texture* checkers_texture;
    ModelTable modelTable;
    std::vector<GameObject*> gameObjects;
    std::vector<glm::vec3> lights;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    void initWindowAPI();
    void initModels();
    void initShaders();
    void initCustom();

public:
    void init();
    void update();
    void destroy();
};