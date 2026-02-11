#pragma once
#include "transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Transform transform;
    Camera(glm::vec3 startPosition) {
        transform.position = startPosition;
    }

    glm::mat4 getViewMatrix() {
        glm::vec3 front;
        front.x = cos(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
        front.y = sin(glm::radians(transform.rotation.x));
        front.z = sin(glm::radians(transform.rotation.y)) * cos(glm::radians(transform.rotation.x));
        front = glm::normalize(front);

        return glm::lookAt(transform.position, transform.position + front, glm::vec3(0.0f, 1.0f, 0.0f));
    }
};