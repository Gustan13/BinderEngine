#include "window.h"

void process_window(GLFWwindow* window, Camera* camera, float deltaTime, GameObject* player) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->yaw -= 0.5f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->yaw += 0.5f;

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        player->playAnimation();
    else
        player->stopAnimation();

    camera->setDirection(cos(glm::radians(camera->yaw)), sin(glm::radians(camera->yaw)));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}