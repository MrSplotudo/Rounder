#pragma once
#include <GLFW/glfw3.h>

#include "transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera;

class ProcessInput {
public:


    void processKeyboard(GLFWwindow* window, float deltaTime, glm::vec3& position, float& yaw, float& pitch, float speed);
    void processMouse(float xOffset, float yOffset, float& yaw, float& pitch);

};