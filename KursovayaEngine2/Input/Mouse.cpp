#include"Mouse.h"
#include"GLFW/glfw3.h"
#include<iostream>



const EventClass& MouseClass::gMousePressableKeyEvent(PressableKeysEnum key) const {
	return PressableKeysEvents[(unsigned int)key];
}

//not for user
void MouseClass::GLFW_KEYCALLBACK(int button, int action, int mods) {
	if (button >= GLFW_MOUSE_BUTTON_LEFT and button <= GLFW_MOUSE_BUTTON_MIDDLE) {
		unsigned int ind = button - GLFW_MOUSE_BUTTON_LEFT;
		PressableKeysStates[ind] = action != GLFW_RELEASE;
		if (action == GLFW_PRESS) { bool v = true; PressableKeysEvents[ind].FireEvent(&v); }
		else if (action == GLFW_RELEASE) { bool v = false; PressableKeysEvents[ind].FireEvent(&v); }
	}
}


bool MouseClass::gMousePressableKeyState(PressableKeysEnum key) const {
	return PressableKeysStates[(unsigned int)key];
}