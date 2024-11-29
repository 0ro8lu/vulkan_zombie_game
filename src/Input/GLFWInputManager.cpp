#include "GLFW/glfw3.h"
#include "IInputManager.h"
#include "GLFWInputManager.h"
#include <tuple>

void GLFWInputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto inputManager = reinterpret_cast<GLFWInputManager *>(glfwGetWindowUserPointer(window));
	
	switch (action) {
    	case GLFW_PRESS: inputManager->currentKeys[key] = true; break;
		case GLFW_RELEASE: inputManager->currentKeys[key] = false; break;
	}
}

void GLFWInputManager::mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {
	auto inputManager = reinterpret_cast<GLFWInputManager*>(glfwGetWindowUserPointer(window));

	InputEvent::Type type;
	switch(action) {
		case GLFW_PRESS: type = InputEvent::Type::MousePress; break;
		case GLFW_RELEASE: type = InputEvent::Type::MouseRelease; break;
	}

	InputEventData data;
	switch (button) {
    	case GLFW_MOUSE_BUTTON_1: data = MouseButton::LEFT; break;
    	case GLFW_MOUSE_BUTTON_2: data = MouseButton::RIGHT; break;
    	case GLFW_MOUSE_BUTTON_3: data = MouseButton::MIDDLE; break;
    }

	inputManager->addEvent(type, data);
}

void GLFWInputManager::mouseposition_callback(GLFWwindow* window, double xpos, double ypos) {
	auto inputManager = reinterpret_cast<GLFWInputManager*>(glfwGetWindowUserPointer(window));

	InputEvent::Type type = InputEvent::Type::MouseMove;
	InputEventData data = std::make_pair(xpos, ypos);
	
	inputManager->addEvent(type, data);
}

GLFWInputManager::GLFWInputManager(GLFWwindow* window, uint num_gamepads) 
	: window(window), maximum_num_gamepads(num_gamepads) {
	
	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, GLFWInputManager::key_callback);
	glfwSetMouseButtonCallback(window, GLFWInputManager::mousebutton_callback);
	glfwSetCursorPosCallback(window, mouseposition_callback);

	inputEvents.reserve(10);
}

GLFWInputManager::~GLFWInputManager() {
	glfwSetWindowUserPointer(window, nullptr);
}

void GLFWInputManager::addEvent(const InputEvent::Type& type, const InputEventData& data) {
	if (writeIndex <= MAX_EVENTS) {
    	if (writeIndex < inputEvents.size()) {
    		// Reuse existing element
    		inputEvents[writeIndex].type = type;
    		inputEvents[writeIndex].data = data;
    	} else {
    		inputEvents.emplace_back(InputEvent{.type = type, .data = data});
    	}
    	writeIndex++;
    }
}

void GLFWInputManager::getGamepadState() {	
	GLFWgamepadstate state;

	for(int i = 0; i < maximum_num_gamepads; i++) {
		if(glfwGetGamepadState(i, &state)) {

			// Process each button using the mapping
            for (const auto& mapping : buttonMappings) {

                // Check if the button is pressed
                if (state.buttons[mapping.glfwButtonIndex] == GLFW_PRESS) {

					inputEvents.emplace_back(InputEvent{
						.type = InputEvent::Type::ControllerButtonPress,
						.data = std::make_tuple(i, mapping.controllerButton)
					});

                } else {

					inputEvents.emplace_back(InputEvent{
						.type = InputEvent::Type::ControllerButtonRelease,
						.data = std::make_tuple(i, mapping.controllerButton)
					});
                }
            }

			for (const auto& mapping : axisMappings) {
                float value = state.axes[mapping.glfwAxisIndex];
				inputEvents.emplace_back(InputEvent{
					.type = InputEvent::Type::ControllerAxisMove,
					.data = std::make_tuple(i, mapping.controllerAxis, value),
				});
            }
		}
	}
}
