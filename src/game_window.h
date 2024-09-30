#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class GameWindow {
public:
    GameWindow(uint32_t width, uint32_t height, std::string title);
    ~GameWindow();

    void setFramebufferCallback(void* userPointer, void(* GLFWframebuffersizefun) (GLFWwindow *window, int width, int height));
    void update();

    uint32_t getWidth() const { return width; }
    uint32_t getHeight() const { return height; }
    GLFWwindow* getWindow() const { return window; }
    bool shouldClose() const { return glfwWindowShouldClose(window); }

private:
    uint32_t width, height;
    std::string title;
    
    GLFWwindow* window;
};

#endif
