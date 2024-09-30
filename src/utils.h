#ifndef _UTILS_H_
#define _UTILS_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <vector>
#include <string>
#include <fstream>

struct StaticUBO{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct alignas(16) DynamicUBO {
    glm::mat4 model;
    float array_index;
};

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

#endif
