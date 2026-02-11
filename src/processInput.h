#pragma once
#include <GLFW/glfw3.h>

#include "transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera;

class ProcessInput {
public:


    void processKeyboard(GLFWwindow* window, float deltaTime, Transform& transform, float speed);
    void processMouse(float xOffset, float yOffset, Transform& transform);
};