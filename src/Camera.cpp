#include "Camera.h"

Camera::Camera(size_t width, size_t height)
    : width(width), height(height) {
}

void Camera::setPosition(glm::vec2 position) {
    cameraPos = position;
    glm::vec3 newCameraPos = glm::vec3(position.x - (float)width / 2, position.y - (float)height / 2, 6);
    cameraLookAt = glm::vec3(position.x, position.y, 0);

    cameraMatrix = glm::lookAt(newCameraPos, newCameraPos + cameraFront, glm::vec3(0, 1, 0));
}

glm::vec2 Camera::convertScreenToWorld(glm::vec2 screenCoords) {

    // Make it so that 0 is the center
    screenCoords -= glm::vec2(width / 2, height / 2);
    screenCoords += cameraPos;
    return screenCoords;
}
