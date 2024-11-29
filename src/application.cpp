#include "Application.h"

#include <chrono>
#include <iostream>
#include <thread>

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

Application::Application() : camera(WINDOW_WIDTH, WINDOW_HEIGHT) {
	// Init window
	gameWindow.reset(new GameWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "zombie_game"));

	// Init Vulkan
	vulkanRenderer.reset(new VulkanRenderer(gameWindow->getWindow(), 8, 100, 2));

	gameView.reset(new GameView(vulkanRenderer, game, camera));

	glfwInputManager.reset(new GLFWInputManager(gameWindow->getWindow(), 4));
}

void Application::run() {

	constexpr auto frame_duration = std::chrono::microseconds(16666);

	// Run renderer and update game (maybe in the reverse order)
	while(!gameWindow->shouldClose()) {

		auto frame_start = std::chrono::steady_clock::now();
		
		// --------- EVENT PROCESSING --------- 
		auto events = glfwInputManager->getEvents();
		for(const auto& iEvent : events) {
			if(iEvent.type == InputEvent::Type::KeyPress && std::get<KeyCode>(iEvent.data) == KeyCode::ESCAPE) {
				gameWindow->close();
			}
		}
		
		if(game.getGameState() == GameState::UNINITIALIZED) {
			game.loadLevel();
			gameView->drawBackground();
		}

		game.update(gameView->processInput(events));

		// Render stuff on screen
		gameView->draw();

		// Measure the elapsed time
        auto frame_end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(frame_end - frame_start);

        // Calculate the remaining time to sleep
        auto sleep_duration = frame_duration - elapsed;

		if (sleep_duration > std::chrono::microseconds::zero()) {
            std::this_thread::sleep_for(sleep_duration);
        } else {
            // If processing takes longer than the frame duration, you might want to handle it (e.g., skip sleep)
            std::cout << "Warning: Frame processing time exceeded frame duration!\n";
        }

		// Optional: Print the actual frame time for debugging
        // auto actual_frame_end = std::chrono::steady_clock::now();
        // auto actual_frame_time = std::chrono::duration_cast<std::chrono::microseconds>(actual_frame_end - frame_start);
        // std::cout << "Frame time: " << actual_frame_time.count() << " microseconds\n";
	}
	vulkanRenderer->wait();
}
