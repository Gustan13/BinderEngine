#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "camera.h"
#include "gameobject.h"

void process_window(GLFWwindow* window, Camera* camera, float deltaTime, GameObject* player);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);