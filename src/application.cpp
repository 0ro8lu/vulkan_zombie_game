#include "application.h"

Application::Application() {
	// Init window
	gameWindow.reset(new GameWindow(800, 600, "zombie_game"));

	// Init Vulkan
	vulkanRenderer.reset(new VulkanRenderer(gameWindow->getWindow(), 8, 100, 2));

	gameGraphicsManager.reset(new GameGraphicsManager(vulkanRenderer));
}

void Application::run() {
	// Run renderer and update game (maybe in the reverse order)
	while(!gameWindow->shouldClose()) {
		// Update the window-events (input)
		gameWindow->update();

		game.update();

		gameGraphicsManager->draw(game.getLevelSprites());

		// Render stuff on screen	
		// vulkanRenderer->drawFrame(game.getObjectData());

	}
	vulkanRenderer->wait();
}
