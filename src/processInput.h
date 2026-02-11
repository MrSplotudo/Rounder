#pragma once
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera;

class ProcessInput {
public:


    void processKeyboard(GLFWwindow* window, float deltaTime, Camera& camera);
    void processMouse(float xOffset, float yOffsetCamera, Camera& camera);
};