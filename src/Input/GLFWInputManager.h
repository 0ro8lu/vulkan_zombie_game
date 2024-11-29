#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include "GLFW/glfw3.h"
#include "IInputManager.h"
#include <unordered_map>

class GLFWInputManager : public IInputManager<GLFWInputManager> {
public:
	GLFWInputManager(GLFWwindow* window, uint num_gamepads);
	~GLFWInputManager();

	const uint maximum_num_gamepads;

	inline std::span<const InputEvent> getEvents() {

		InputEvent::Type type;
		for (const std::pair<unsigned int, bool>& n : currentKeys) {
			auto key = n.first;
			auto value = n.second; 

			if(value == false) {
				if(previousKeys[key] == true) {
					type = InputEvent::Type::KeyRelease;
				} else {
					continue;
				}
			}
			if(value == true) {
				type = InputEvent::Type::KeyPress;
			}

			InputEventData data;
 			switch(key) {
 				case GLFW_KEY_W: data = KeyCode::W; break;
				case GLFW_KEY_ENTER: data = KeyCode::RETURN; break;
 				case GLFW_KEY_A: data = KeyCode::A; break;
 				case GLFW_KEY_S: data = KeyCode::S; break;
 				case GLFW_KEY_D: data = KeyCode::D; break;
 				case GLFW_KEY_ESCAPE: data = KeyCode::ESCAPE; break;
				default: data = KeyCode::CONTROL; break;
			}

    		// inputEvents.emplace_back(InputEvent{.type = type, .data = data});
			addEvent(type, data);
		}

		// copy current into previous
		previousKeys = currentKeys;

		glfwPollEvents();
		// getGamepadState();

		std::span<InputEvent> returnspan = inputEvents;
		returnspan = returnspan.first(writeIndex);
		writeIndex = 0;

		return returnspan;
	}

private:
	GLFWwindow* window;

	void getGamepadState();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	std::unordered_map<unsigned int, bool> currentKeys;
	std::unordered_map<unsigned int, bool> previousKeys;
	
	static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouseposition_callback(GLFWwindow* window, double xpos, double ypos);

	std::vector<InputEvent> inputEvents;
	static constexpr size_t MAX_EVENTS = 20;
	size_t writeIndex = 0;

	void addEvent(const InputEvent::Type& eventType, const InputEventData& data);

	struct ButtonMapping {
    	int glfwButtonIndex;
    	ControllerButton controllerButton;
    };

	const std::array<ButtonMapping, 15> buttonMappings = {{
    	{ GLFW_GAMEPAD_BUTTON_A, ControllerButton::A },
    	{ GLFW_GAMEPAD_BUTTON_B, ControllerButton::B },
    	{ GLFW_GAMEPAD_BUTTON_X, ControllerButton::X },
    	{ GLFW_GAMEPAD_BUTTON_Y, ControllerButton::Y },
    	{ GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, ControllerButton::LeftBumper },
    	{ GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER, ControllerButton::RightBumper },
    	{ GLFW_GAMEPAD_BUTTON_BACK, ControllerButton::Back },
    	{ GLFW_GAMEPAD_BUTTON_START, ControllerButton::Start },
    	{ GLFW_GAMEPAD_BUTTON_GUIDE, ControllerButton::Guide },
    	{ GLFW_GAMEPAD_BUTTON_DPAD_UP, ControllerButton::DPadUp },
    	{ GLFW_GAMEPAD_BUTTON_DPAD_RIGHT, ControllerButton::DPadRight },
    	{ GLFW_GAMEPAD_BUTTON_DPAD_DOWN, ControllerButton::DPadDown },
    	{ GLFW_GAMEPAD_BUTTON_DPAD_LEFT, ControllerButton::DPadLeft },
    	{ GLFW_GAMEPAD_BUTTON_LEFT_THUMB, ControllerButton::LeftThumb },
    	{ GLFW_GAMEPAD_BUTTON_RIGHT_THUMB, ControllerButton::RightThumb }
    }};

	struct AxisMapping {
    	int glfwAxisIndex;
    	ControllerAxis controllerAxis;
    };

	const std::array<AxisMapping, 6> axisMappings = {{
    	{GLFW_GAMEPAD_AXIS_LEFT_X, ControllerAxis::LeftX },
    	{GLFW_GAMEPAD_AXIS_LEFT_Y, ControllerAxis::LeftY },
    	{GLFW_GAMEPAD_AXIS_RIGHT_X, ControllerAxis::RightX },
    	{GLFW_GAMEPAD_AXIS_RIGHT_Y, ControllerAxis::RightY },
    	{GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, ControllerAxis::LeftTrigger },
    	{GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, ControllerAxis::RightTrigger }
    }};
};


#endif
