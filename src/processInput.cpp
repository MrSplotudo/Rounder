#include "processInput.h"
#include "camera.h"
#include "glm/gtc/quaternion.hpp"


void ProcessInput::processKeyboard(GLFWwindow* window, float deltaTime, Camera& camera) {
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw));
    front.y = 0.0f;
    front.z = sin(glm::radians(camera.yaw));
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    float velocity = camera.speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.position.y += velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.position.y -= velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.speed = 6.0f;
    } else {
        camera.speed = 2.5f;
    }
}

void ProcessInput::processMouse(float xOffset, float yOffset, Camera& camera) {
    float sensitivity = 0.1f;
    camera.yaw += xOffset * sensitivity;
    camera.pitch -= yOffset * sensitivity;

    if (camera.pitch > 89.0f) camera.pitch = 89.0f;
    if (camera.pitch < -89.0f) camera.pitch = -89.0f;
}