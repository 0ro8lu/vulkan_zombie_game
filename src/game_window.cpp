#include "game_window.h"

GameWindow::GameWindow(uint32_t width, uint32_t height, std::string title)
    : width(width), height(height), title(title) {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
}

GameWindow::~GameWindow() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void GameWindow::setFramebufferCallback(void* userPointer, void(* GLFWframebuffersizefun) (GLFWwindow *window, int width, int height)) {
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, GLFWframebuffersizefun);
}

void GameWindow::update() {
    glfwPollEvents();
}
