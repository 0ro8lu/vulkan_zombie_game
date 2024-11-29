#include "GameWindow.h"

uint32_t GameWindow::width = 0;
uint32_t GameWindow::height = 0;
std::string GameWindow::title = "";

GameWindow::GameWindow(uint32_t width, uint32_t height, std::string title) {
  glfwInit();

  GameWindow::width = width;
  GameWindow::height = height;
  GameWindow::title = title;

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
