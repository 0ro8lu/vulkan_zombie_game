#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "game_window.h"
#include "vulkan_renderer.h"
#include "game_graphics_manager.h"
#include "game.h"

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
    std::unique_ptr<GameGraphicsManager> gameGraphicsManager;
    Game game;
};

#endif
