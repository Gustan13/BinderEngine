#include "game.h"

void Game::init() {
	initWindowAPI();
	initShaders();
	initModels();
    initCustom();
}

void Game::initWindowAPI() {
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    printf("%s\n", glfwGetVersionString());

    window = glfwCreateWindow(800, 600, "Lord of the Damned", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    camera = new Camera;
    lightSource = new Cube;
    lightSource->setupCube();
}

void Game::initModels() {
    modelTable.loadModel("D:/3DModels/dancing_vampire.dae", "vamp");
    modelTable.loadModel("D:/3DModels/pawn.dae", "pawn");
}

void Game::initShaders() {
    litShader = new Shader("anim_vshader.txt", "lit_fshader.txt");
    lightShader = new Shader("lit_vshader.txt", "light_fshader.txt");
    checkers_texture = new Texture("D:/wall.jpg");
    glEnable(GL_DEPTH_TEST);
}

void Game::initCustom() {
    GameObject* testObject = new GameObject;
    testObject->modelName = "vamp";
    gameObjects.push_back(testObject);

    testObject->currentModel = modelTable.findModel(testObject->modelName);

    glm::vec3 lightPositions[] = {
        glm::vec3(1.f,0.f,-1.f),
        glm::vec3(4.f,0.9f,1.5f),
        glm::vec3(1.6f,1.f,5.f),
        glm::vec3(5.f,3.3f,1.1f)
    };

    for (glm::vec3 pos : lightPositions)
        lights.push_back(pos);
}

void Game::update() {
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float currentFrameMilli = ((float)glfwGetTime()) / 1000.f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_window(window, camera, deltaTime, gameObjects.front());

        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

        lightShader->use();
        lightShader->setMatrix("view", glm::value_ptr(camera->getViewMatrix()));
        lightShader->setMatrix("projection", glm::value_ptr(projection));
        for (int i = 0; i < lights.size(); ++i) {
            lightSource->setPosition(lights[i]);
            lightSource->setScale(glm::vec3(.1f, .1f, .1f));
            lightSource->drawCube(lightShader);
        }

        litShader->use();
        litShader->setVec3("viewPos", glm::value_ptr(camera->getPosition()));
        litShader->setVec3("objectColor", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));

        for (int i = 0; i < lights.size(); ++i) {
            std::string num = std::to_string(i);
            litShader->setVec3(("lights[" + num + "].position").c_str(), glm::value_ptr(lights[i]));
            litShader->setVec3(("lights[" + num + "].ambient").c_str(), glm::value_ptr(glm::vec3(0.f, 0.f, 0.f)));
            litShader->setVec3(("lights[" + num + "].diffuse").c_str(), glm::value_ptr(glm::vec3(1, 1, 1)));
            litShader->setVec3(("lights[" + num + "].specular").c_str(), glm::value_ptr(glm::vec3(.5f, .5f, .5f)));
            litShader->setFloat(("lights[" + num + "].constant").c_str(), 1.0f);
            litShader->setFloat(("lights[" + num + "].linear").c_str(), 0.09f);
            litShader->setFloat(("lights[" + num + "].quadratic").c_str(), 0.01f);
        }

        litShader->setInt("nr_lights", lights.size());
        litShader->setMatrix("view", glm::value_ptr(camera->getViewMatrix()));
        litShader->setMatrix("projection", glm::value_ptr(projection));

        checkers_texture->activate(GL_TEXTURE0);
        checkers_texture->bind();

        for (GameObject* object : gameObjects) {
            if (!object->currentModel)
                continue;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, object->position);
            model = glm::scale(model, object->scale);
            model = glm::rotate(model, glm::radians(object->rotation.x), glm::vec3(1.f, 0.f, 0.f));
            model = glm::rotate(model, glm::radians(object->rotation.y), glm::vec3(0.f, 1.f, 0.f));
            model = glm::rotate(model, glm::radians(object->rotation.z), glm::vec3(0.f, 0.f, 1.f));
            litShader->setMatrix("model", glm::value_ptr(model));

            if (object->playingAnimation) {
                object->animTimer += deltaTime;

                object->currentModel->boneTransform(object->animTimer);
                for (unsigned int i = 0; i < object->currentModel->GetBoneCount(); ++i)
                    litShader->setMatrix(("gBones[" + std::to_string(i) + "]").c_str(), glm::value_ptr(object->currentModel->transforms[i]));
            }

            object->update();
            object->currentModel->draw(litShader);
        }
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Game::destroy() {
    delete lightSource;

    for (GameObject* gameObject : gameObjects)
        delete gameObject;

    gameObjects.clear();

    glDeleteProgram(litShader->ID);
    glDeleteProgram(lightShader->ID);

    glfwTerminate();
}