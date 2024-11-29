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

    static uint32_t getWidth() { return width; }
    static uint32_t getHeight() { return height; }
    GLFWwindow* getWindow() const { return window; }

    void close() { glfwSetWindowShouldClose(window, 1); }
    bool shouldClose() const { return glfwWindowShouldClose(window); }

private:
    static uint32_t width, height;
    static std::string title;
    
    GLFWwindow* window;
};

#endif
