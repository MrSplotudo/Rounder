#include "processInput.h"
#include "transform.h"
#include "glm/gtc/quaternion.hpp"


void ProcessInput::processKeyboard(GLFWwindow* window, float deltaTime, glm::vec3& position, float& yaw, float& pitch, float speed) {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw));
    front.y = 0.0f;
    front.z = sin(glm::radians(yaw));
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    float velocity = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position.y += velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        position.y -= velocity;
}

void ProcessInput::processMouse(float xOffset, float yOffset, float& yaw, float& pitch) {
    float sensitivity = 0.1f;
    yaw += xOffset * sensitivity;
    pitch -= yOffset * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}
