#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "GameWindow.h"
#include "vulkan_renderer.h"
#include "GameView.h"
#include "Input/GLFWInputManager.h"
#include "ZombieGame/ZombieGame.h"

#include <memory>

class Application {
public:
    Application();
    
    void run();
    void cleanup();

private:
    // smart pointer because i'd have to declare a copy constructor instead ^^ 
    std::unique_ptr<VulkanRenderer> vulkanRenderer;
    std::unique_ptr<GameWindow> gameWindow;
    std::unique_ptr<GameView> gameView;
	std::unique_ptr<GLFWInputManager> glfwInputManager;

	Camera camera;
    ZombieGame game;
};

#endif
