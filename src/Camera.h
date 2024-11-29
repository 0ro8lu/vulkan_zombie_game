#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

class Camera {
public:
    Camera(size_t width, size_t height);
    void setPosition(glm::vec2 position);
    const glm::vec3& getLookAt() { return cameraLookAt; };
    const glm::mat4& getCameraMatrix() const { return cameraMatrix; }
    glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

private:
    // glm::mat4 orthographic_camera;
    // glm::mat4 camera_matrix;
    glm::mat4 cameraMatrix;
    glm::vec3 cameraLookAt;
    glm::vec2 cameraPos;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    size_t width;
    size_t height;
};

#endif
