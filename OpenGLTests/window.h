#pragma once

#include <GLFW/glfw3.h>
#include "camera.h"

void process_window(GLFWwindow* window, Camera* camera, float deltaTime);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);