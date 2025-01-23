#define STB_IMAGE_IMPLEMENTATION
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

const float radius = 10.f;

glm::vec3 positions[] = {
    glm::vec3(0.f,0.f,-1.f),
    glm::vec3(0.f,2.9f,0.5f),
    glm::vec3(0.6f,1.f,3.f),
    glm::vec3(2.f,1.3f,1.1f)
};

glm::vec3 lightPositions[] = {
    glm::vec3(1.f,0.f,-1.f),
    glm::vec3(4.f,0.9f,1.5f),
    glm::vec3(1.6f,1.f,5.f),
    glm::vec3(5.f,3.3f,1.1f)
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

int main(void)
{
    GLFWwindow* window;
    Camera* camera;
    Cube* cube, *lightSource;
    Shader* shader, *litShader, *lightShader;
    Texture* checkers_texture;

    camera = new Camera();
    cube = new Cube();
    lightSource = new Cube();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    printf("%s\n", glfwGetVersionString());

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Lord of the Damned", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //litShader = new Shader("lit_vshader.txt", "lit_fshader.txt");
    litShader = new Shader("anim_vshader.txt", "lit_fshader.txt");
    lightShader = new Shader("lit_vshader.txt", "light_fshader.txt");
    cube->setupCube();
    lightSource->setupCube();

    checkers_texture = new Texture("D:/wall.jpg");

    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //Model* pawn = new Model("D:/3DModels/type_81_assault_rifle.glb");
    //Model* pawn = new Model("D:/3DModels/Metal Gear Solid Intergral - Solid Snake.obj");
    //Model* pawn = new Model("D:/3DModels/clicker-repaired.stl");
    Model* pawn = new Model("D:/3DModels/dancing_vampire.dae");
    //Model* pawn = new Model("D:/3DModels/pawn.dae");

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float currentFrameMilli = ((float)glfwGetTime()) / 1000.f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_window(window, camera, deltaTime);
        //animator.UpdateAnimation(deltaTime);

        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);
        
        lightShader->use();
        lightShader->setMatrix("view", glm::value_ptr(camera->getViewMatrix()));
        lightShader->setMatrix("projection", glm::value_ptr(projection));
        for (int i = 0; i < 4; ++i) {
            lightSource->setPosition(lightPositions[i]);
            lightSource->setScale(glm::vec3(1.f, .1f, .1f));
            lightSource->drawCube(lightShader);
        }

        litShader->use();
        litShader->setVec3("viewPos", glm::value_ptr(camera->getPosition()));
        litShader->setVec3("objectColor", glm::value_ptr(glm::vec3(1.f,1.f,1.f)));

        for (int i = 0; i < 4; ++i) {
            std::string num = std::to_string(i);
            litShader->setVec3(("lights[" + num + "].position").c_str(), glm::value_ptr(lightPositions[i]));
            litShader->setVec3(("lights[" + num + "].ambient").c_str(), glm::value_ptr(glm::vec3(0.f, 0.f, 0.f)));
            litShader->setVec3(("lights[" + num + "].diffuse").c_str(), glm::value_ptr(glm::vec3(1, 1, 1)));
            litShader->setVec3(("lights[" + num + "].specular").c_str(), glm::value_ptr(glm::vec3(.5f, .5f, .5f)));
            litShader->setFloat(("lights[" + num + "].constant").c_str(), 1.0f);
            litShader->setFloat(("lights[" + num + "].linear").c_str(), 0.09f);
            litShader->setFloat(("lights[" + num + "].quadratic").c_str(), 0.01f);
        }

        litShader->setInt("nr_lights", 4);
        litShader->setMatrix("view", glm::value_ptr(camera->getViewMatrix()));
        litShader->setMatrix("projection", glm::value_ptr(projection));

        /*auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            litShader->setMatrix("finalBonesMatrices[" + std::to_string(i) + "]", glm::value_ptr(transforms[i]));*/

        pawn->boneTransform(currentFrame);
        for (unsigned int i = 0; i < pawn->GetBoneCount(); ++i)
            litShader->setMatrix(("gBones[" + std::to_string(i) + "]").c_str(), glm::value_ptr(pawn->transforms[i]));
        //litShader->setMatrix("gBones[" + std::to_string(i) + "]", glm::value_ptr(glm::mat4(1.f)));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.f, -1.0f));
        model = glm::scale(model, glm::vec3(.01f, .01f, .01f));
        //model = glm::rotate(model, glm::radians(-90.f),glm::vec3(1.0f, 0.f, 0.0f));
        litShader->setMatrix("model", glm::value_ptr(model));

        checkers_texture->activate(GL_TEXTURE0);
        checkers_texture->bind();

        for (int i = 0; i < 4; ++i) {
            /*glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, positions[i]);
            model = glm::scale(model, glm::vec3(0.3f,0.3f,0.3f));
            litShader->setMatrix("model", glm::value_ptr(model));
            pawn->draw(litShader);*/

            /*cube->setPosition(positions[i]);
            cube->setRotation(glm::vec3(50.f, 10.f, 40.f));*/
            //cube->drawCube(litShader);
        }

        /*glm::mat4 model = glm::mat4(1.f);
        model = glm::translate(model, positions[0]);
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));*/
        //model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1, 0, 0));
        //litShader->setMatrix("model", glm::value_ptr(model));
        pawn->draw(litShader);
        //std::cout << "bro\n";
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete cube;
    
    glDeleteProgram(litShader->ID);
    glDeleteProgram(lightShader->ID);

    glfwTerminate();
    return 0;
}