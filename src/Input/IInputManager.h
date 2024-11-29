#ifndef I_INPUT_MANANGER_H_
#define I_INPUT_MANANGER_H_

#include <variant>
// #include <vector>
#include <span>

enum class KeyCode {
	A, B, C, D, E, F, G, H, I, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
	ESCAPE, RETURN, DELETE, CONTROL, SHIFT, TAB,
	RIGHT_ARROW, LEFT_ARROW, UP_ARROW, DOWN_ARROW, 
};

enum class MouseButton {
	LEFT, RIGHT, MIDDLE,	
};

enum class ControllerButton {
	A, B, Y, X, LeftBumper, RightBumper,
	Back, Start, Guide, LeftThumb, RightThumb,
	DPadUp, DPadDown, DPadLeft, DPadRight,	
};

enum class ControllerAxis {
	LeftX, LeftY, RightX, RightY, LeftTrigger, RightTrigger,
};

typedef std::variant<
        KeyCode,
        MouseButton,
        std::pair<double, double>, // For mouse move
        std::tuple<int, ControllerButton>, // Controller ID and button
        std::tuple<int, ControllerAxis, float>> InputEventData; // Controller ID, axis, value

struct InputEvent {
	enum class Type {
		KeyPress,
        KeyRelease,
		KeyRepeat,
        MousePress,
        MouseRelease,
        MouseMove,
        ControllerButtonPress,
        ControllerButtonRelease,
        ControllerAxisMove,	
	} type;

	InputEventData data;
};

template<typename Derived>
class IInputManager {
public:
	inline std::span<const InputEvent> getEvents() {
		return impl().getEvents();
	}

private:
	inline Derived& impl() {
		return *static_cast<Derived*>(this);
	}	
};

#endif
