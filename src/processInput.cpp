#include "processInput.h"
#include "transform.h"
#include "glm/gtc/quaternion.hpp"


void ProcessInput::processKeyboard(GLFWwindow* window, float deltaTime, Transform& transform, float speed) {
    float yaw = transform.rotation.y;
    glm::vec3 front;
    front.x = cos(glm::radians(yaw));
    front.y = 0.0f;
    front.z = sin(glm::radians(yaw));
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    float velocity = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        transform.position.y += velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        transform.position.y -= velocity;
}

void ProcessInput::processMouse(float xOffset, float yOffset, Transform& transform) {
    float sensitivity = 0.1f;
    transform.rotation.y += xOffset * sensitivity;
    transform.rotation.x -= yOffset * sensitivity;

    if (transform.rotation.x > 89.0f) transform.rotation.x = 89.0f;
    if (transform.rotation.x < -89.0f) transform.rotation.x = -89.0f;
}